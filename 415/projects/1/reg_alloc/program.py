from reg_alloc.instruction import Instruction
from reg_alloc.register import Register, PRegister, VRegister
import logging

logging.basicConfig()

class Program:

    def __init__(self, filename):
        self.instrs = []
        self.vregs = []
        self.pregs = []
        self.spilloff = 0
        self.logger = logging.getLogger(__name__)
        self.logger.setLevel(logging.INFO)

        with open(filename, "r") as f:
            linenum = 0
            for line in f:
                if line.startswith("//") or line.isspace():
                    continue
                i = Instruction(line, linenum, self.vregs)
                self.instrs.append(i)
                linenum += 1

    def allocate_registers(self, algo, numregs):
        numregs = int(numregs)
        if numregs > len(self.vregs):
            return self.top_down1(numregs)
        if algo == "s":
            return self.top_down1(numregs)
        elif algo == "t":
            return self.top_down2(numregs)
        elif algo == "b":
            return self.bottom_up(numregs)
        else:
            raise ValueError("Invalid algorithm %s" % algo)

    def top_down1(self, numregs):
        feasible_set = 3
        for i in range(0, numregs):
            self.pregs.append(PRegister(i))

        self.vregs.sort(reverse=True, key=lambda r: r.occurances)

        # Add loading the base register
        load_base = "loadI 1020 => %s" % str(self.pregs[0])
        self.instrs.insert(0, Instruction(load_base, 0, [], isphys=True))

        v2p = {}
        min_num = min(numregs-feasible_set, len(self.vregs))
        for i in range(0, min_num):
            v2p[self.vregs[i]] = self.pregs[i + feasible_set]

        spilloff = -4
        for i in range(min_num, len(self.vregs)):
            v2p[self.vregs[i]] = spilloff
            spilloff -= 4

        # Allocate registers
        self.gen_code_from_mapping(v2p)

    def top_down2(self, numregs):
        feasible_set = 3
        for i in range(0, numregs):
            self.pregs.append(PRegister(i))

        live_regs = []
        for i in range(0, len(self.instrs)):
            live_regs.append((i, []))
            for reg in self.vregs:
                if i in range(reg.live_range[0], reg.live_range[1] + 1):
                    live_regs[i][1].append(reg)

        live_regs.sort(reverse=True, key=lambda l: len(l[1]))
        v2p = {}
        spilloff = 0

        for line in live_regs:
            line = line[1]
            if len(line) <= numregs - feasible_set:
                break
            while Program.not_spilled(line, v2p) > numregs - feasible_set:
                # Need to spill. Pick reg with most occurances
                # If tied, pick longest live range
                spillreg = line[0]
                for reg in line:
                    if reg in v2p:
                        continue
                    if reg.occurances <= spillreg.occurances:
                        spillreg = reg
                    elif reg.occurances == spillreg.occurances:
                        if reg.live_range[1] - reg.live_range[0] > spillreg.live_range[1] - spillreg.live_range[0]:
                            spillreg = reg
                v2p[spillreg] = spilloff
                spillreg.spilloff = spilloff
                spilloff -= 4

        for line in self.instrs:
            regs = line.get_regs()
            for reg in regs[0] + regs[1]:
                if isinstance(reg, PRegister):
                    continue
                if reg.spilloff >= 0 and not reg in v2p:
                    preg = self.get_available_preg_td(reg)
                    if preg is None:
                        print "ERROR"
                    preg.vreg = reg
                    reg.preg = preg
                    v2p[reg] = preg

        # Add loading the base register
        load_base = "loadI 1020 => %s" % str(self.pregs[0])
        self.instrs.insert(0, Instruction(load_base, 0, [], isphys=True))

        # Allocate registers
        self.gen_code_from_mapping(v2p)


    def bottom_up(self, numregs):
        feasible_set = 2
        for i in range(0, numregs):
            self.pregs.append(PRegister(i))

        # Add loading the base register
        load_base = "loadI 1020 => %s" % str(self.pregs[0])
        self.instrs.insert(0, Instruction(load_base, 0, [], isphys=True))

        linenum = 1
        orig_linenum = 0

        while linenum < len(self.instrs):
            line = self.instrs[linenum]
            self.logger.debug("LINE: %s", str(line))

            curr_regs = line.get_regs()

            # Ensure all sources are loaded
            src_regs = curr_regs[0]
            if line.opcode == "store":
                src_regs += curr_regs[1]

            insert = []
            for reg in src_regs:
                if isinstance(reg, PRegister):
                        continue
                if not reg.preg:
                    # Need to load, see if there are any available registers
                    preg = self.get_available_preg()
                    if not preg:
                        # Need to spill something
                        preg = self.get_far_preg(orig_linenum)
                        self.logger.debug('spilling %s for %s', str(preg.vreg),
                                str(reg))
                        insert += self.spillreg_bu(preg)
                        preg.vreg = None
                    if reg.spilloff <= 0:
                        insert += Program.loadreg(reg.spilloff, preg)
                    self.logger.debug("Mapping preg %s to %s", str(preg),
                            str(reg))
                    preg.vreg = reg
                    reg.preg = preg
                for i in range(0, len(line.src)):
                    if line.src[i] == reg:
                        line.src[i] = reg.preg
                if line.opcode == "store":
                    if line.dst[0] == reg:
                        line.dst[0] = reg.preg

            for reg in src_regs:
                if isinstance(reg, PRegister):
                        continue
                if reg.live_range[1] < orig_linenum and reg.preg:
                    self.logger.debug("Done using vreg %s, preg %s", str(reg),
                            str(reg.preg))
                    reg.preg.vreg = None

            # Insert any code before instr
            for i in insert:
                self.instrs.insert(linenum, i)
                linenum += 1

            insert = []
            # Figure out where to put output
            if not line.opcode == "store" and len(line.dst) > 0:
                reg = line.dst[0]
                if not isinstance(line.dst[0], PRegister):
                    preg = self.get_available_preg()
                    self.logger.debug("Got preg %s for vreg %s",
                            str(preg), str(reg))
                    if not preg:
                        # Need to spill another
                        preg = self.get_far_preg(orig_linenum)
                        insert += self.spillreg_bu(preg)
                        preg.vreg = None
                    reg.preg = preg
                    line.dst[0] = reg.preg
                    preg.vreg = reg
                    self.logger.debug("Mapping preg %s to %s", str(preg),
                            str(reg))


            # Insert any code after instr
            for i in insert:
                self.instrs.insert(linenum, i)
                linenum += 1

            linenum += 1
            orig_linenum += 1


    def spillreg_bu(self, preg):
        preg.vreg.preg = None
        if preg.vreg.spilloff <= 0:
            self.logger.debug('%s already spilled, not generating spill',
                    str(preg.vreg))
            return []

        spilloff = self.spilloff
        preg.vreg.spilloff = spilloff
        self.spilloff -= 4
        return Program.spillreg(spilloff, preg, self.pregs[1])


    def get_available_preg_td(self, reg):
        for preg in self.pregs[3:]:
            if not preg.vreg:
                return preg
            v = preg.vreg
            vr = range(v.live_range[0], v.live_range[1] + 1)
            rr = range(reg.live_range[0], reg.live_range[1] + 1)

            if not set(vr) & set(rr):
                return preg
        return None


    def get_available_preg(self):
        for preg in self.pregs[2:]:
            if not preg.vreg:
                return preg
            #while len(preg.vreg.uses) > 0 and preg.vreg.uses[0] < linenum:
            #    preg.vreg.uses.pop(0)
            #if len(preg.vreg.uses) == 0:
            #    preg.vreg = None
            #    return preg
        return None

    def get_far_preg(self, linenum):
        ret = self.pregs[2]
        for preg in self.pregs[2:]:
            while len(preg.vreg.uses) != 0 and preg.vreg.uses[0] < linenum:
                preg.vreg.uses.pop(0)
            if len(preg.vreg.uses) == 0:
                return preg
            if preg.vreg.uses[0] > ret.vreg.uses[0]:
                ret = preg
        return ret


    def gen_code_from_mapping(self, v2p):
        # print mappings

        linenum = 1
        for orig_linenum in range(1, len(self.instrs)):
            line = self.instrs[linenum]

            feas_num = 1
            curr_regs = line.get_regs()

            src_regs = curr_regs[0]
            if line.opcode == "store":
                src_regs += curr_regs[1]

            # Load any src regs that are not in registers
            insert = []
            for reg in src_regs:
                if isinstance(reg, PRegister):
                    continue
                if not isinstance(v2p[reg], Register):
                    # Need to load
                    insert += Program.loadreg(v2p[reg], self.pregs[feas_num])
                    reg.preg = self.pregs[feas_num]
                    feas_num += 1
                else:
                    reg.preg = v2p[reg]

            # Insert load code
            for i in insert:
                self.instrs.insert(linenum, i)
                linenum += 1

            # Replace src regs with pregs
            for r in range(0, len(line.src)):
                reg = line.src[r]
                if not isinstance(reg, VRegister):
                    continue
                line.src[r] = reg.preg

            # Replace dst regs with pregs, or feas
            spill = False
            if len(line.dst) > 0:
                dst = line.dst[0]
                if isinstance(dst, VRegister):
                    if isinstance(v2p[dst], Register):
                        dst.preg = v2p[dst]
                    if dst.preg:
                        line.dst[0] = dst.preg
                    else:
                        line.dst[0] = self.pregs[1]
                        spill = True

            linenum += 1

            # Spill if needed
            if spill:
                insert = Program.spillreg(v2p[dst], line.dst[0], self.pregs[2])
                for i in insert:
                    self.instrs.insert(linenum, i)
                    linenum += 1



    @staticmethod
    def not_spilled(active, mapping):
        count = 0
        for r in active:
            if not r in mapping:
                count += 1
        return count

    @staticmethod
    def spillreg(spilloff, reg, freg):
        instrs = []
        i1 = "addI r0, %d => %s" % (spilloff, str(freg))
        i2 = "store %s => %s" % (str(reg), str(freg))
        instrs.append(Instruction(i1, 0, [], isphys=True))
        instrs.append(Instruction(i2, 0, [], isphys=True))
        return instrs

    @staticmethod
    def loadreg(spilloff, freg):
        instrs = []
        i1 = "addI r0, %d => %s" % (spilloff, str(freg))
        i2 = "load %s => %s" % (str(freg), str(freg))
        instrs.append(Instruction(i1, 0, [], isphys=True))
        instrs.append(Instruction(i2, 0, [], isphys=True))
        return instrs
