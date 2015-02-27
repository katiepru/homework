from reg_alloc.instruction import Instruction
from reg_alloc.register import Register, PRegister, VRegister

class Program:

    def __init__(self, filename):
        self.instrs = []
        self.vregs = []
        self.pregs = []
        self.spilloff = 0

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
        algo = int(algo)
        if numregs > len(self.vregs):
            return self.top_down1(numregs)
        if algo == 0:
            return self.top_down1(numregs)
        elif algo == 1:
            return self.top_down2(numregs)
        elif algo == 2:
            return self.bottom_up(numregs)
        else:
            raise ValueError("Invalid algorithm %d" % algo)

    def top_down1(self, numregs):
        feasible_set = 3
        for i in range(0, numregs):
            self.pregs.append(PRegister(i))

        self.vregs.sort(reverse=True, key=lambda r: r.occurances)

        # Add loading the base register
        load_base = "loadI 1020 => %s" % str(self.pregs[0])
        self.instrs.insert(0, Instruction(load_base, 0, [], isphys=True))

        v2p = {}
        for i in range(0, numregs - feasible_set + 1):
            v2p[self.vregs[i]] = PRegister(i + feasible_set)

        spilloff = -4
        for i in range(numregs - feasible_set + 1, len(self.vregs)):
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
        spilloff = -4

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

        for l in live_regs:
            line = l[1]
            linenum = l[0]
            for reg in line:
                if reg.spilloff >= 0 and not reg in v2p:
                    preg = self.get_available_preg_td(linenum)
                    if preg is None:
                        print "FUCK"
                    preg.vreg = reg
                    reg.preg = preg
                    v2p[reg] = preg

        # Add loading the base register
        load_base = "loadI 1020 => %s" % str(self.pregs[0])
        self.instrs.insert(0, Instruction(load_base, 0, [], isphys=True))

        # Allocate registers
        self.gen_code_from_mapping(v2p)


    def bottom_up(self, numregs):
        feasible_set = 3
        for i in range(0, numregs):
            self.pregs.append(PRegister(i))

        # Add loading the base register
        load_base = "loadI 1020 => %s" % str(self.pregs[0])
        self.instrs.insert(0, Instruction(load_base, 0, [], isphys=True))

        linenum = 1
        orig_linenum = 0
        while linenum < len(self.instrs):
            line = self.instrs[linenum]
            curr_vregs = line.get_regs()
            feas_num = 1

            insert_instrs = []
            for dst in curr_vregs[1]:
                if isinstance(dst, PRegister):
                    continue
                if "store" in line.opcode:
                    # Load if needed
                    if dst.spilloff < 0:
                        i = Program.loadreg(dst.spilloff,
                                self.pregs[feas_num])
                        insert_instrs += i
                        dst.preg = self.pregs[feas_num]
                        dst.preg.vreg = dst
                    # Replace vreg with preg
                    for i in range(0, len(line.dst)):
                        if line.dst[i] == dst:
                            line.dst[i] = dst.preg
                else:
                    dst_preg = self.get_available_preg(orig_linenum)
                    if dst_preg is None:
                        # Need to spill something
                        spill_preg = self.get_far_preg(orig_linenum)
                        i = Program.spillreg(self.spilloff, spill_preg,
                                self.pregs[feas_num])
                        insert_instrs += i
                        spill_preg.vreg.spilloff = self.spilloff
                        self.spilloff -= 4
                        feas_num += 1
                        spill_preg.vreg.preg = None
                        spill_preg.vreg = dst
                        dst_preg = spill_preg
                    # Replace with preg
                    dst_preg.vreg = dst
                    dst.preg = dst_preg
                    for i in range(0, len(line.dst)):
                        if line.dst[i] == dst:
                            line.dst[i] = dst_preg
                            break

            for src in curr_vregs[0]:
                if isinstance(src, PRegister):
                    continue
                if src.spilloff < 0:
                    # Load into feasible set
                    i = Program.loadreg(src.spilloff, self.pregs[feas_num])
                    src.preg = self.pregs[feas_num]
                    feas_num += 1
                    insert_instrs += i
                # Replace vreg with preg
                for i in range(0, len(line.src)):
                    if line.src[i] == src:
                        line.src[i] = src.preg

            for insert in insert_instrs:
                self.instrs.insert(linenum, insert)
                linenum += 1

            linenum += 1
            orig_linenum += 1


    def get_available_preg_td(self, linenum):
        for preg in self.pregs[3:]:
            if not preg.vreg:
                return preg
            v = preg.vreg
            if not linenum in range(v.live_range[0], v.live_range[1] + 1):
                return preg
        return None


    def get_available_preg(self, linenum):
        for preg in self.pregs[3:]:
            if not preg.vreg:
                return preg
            while len(preg.vreg.uses) > 0 and preg.vreg.uses[0] < linenum:
                preg.vreg.uses.pop(0)
            if len(preg.vreg.uses) == 0:
                preg.vreg = None
                return preg
        return None

    def get_far_preg(self, linenum):
        ret = self.pregs[3]
        for preg in self.pregs[3:]:
            while len(preg.vreg.uses) == 0 and preg.vreg.uses[0] < linenum:
                preg.vreg.uses.pop(0)
            if preg.vreg.uses[0] > ret.vreg.uses[0]:
                ret = preg
        return ret


    def gen_code_from_mapping(self, v2p):
        for k, v in v2p.iteritems():
            print "%s => %s" % (str(k), str(v))
        linenum = 1
        while linenum < len(self.instrs):
            line = self.instrs[linenum]
            curr_vregs = line.get_regs()
            feas_num = 1
            for src_vreg in curr_vregs[0]:
                if not isinstance(v2p[src_vreg], Register):
                    if isinstance(src_vreg, VRegister):
                        # Load
                        insert = Program.loadreg(v2p[src_vreg],
                                                self.pregs[feas_num])
                        for i in insert:
                            self.instrs.insert(linenum, i)
                            linenum += 1

                        # Replace vreg with feas reg
                        if isinstance(line.src[0], Register) and line.src[0].num == src_vreg.num:
                            line.src[0] = self.pregs[feas_num]
                        else:
                            line.src[1] = self.pregs[feas_num]

                        feas_num += 1
                else:
                    # Replace vreg with preg
                    if isinstance(line.src[0], Register) and line.src[0].num == src_vreg.num:
                        line.src[0] = v2p[src_vreg]
                    else:
                        line.src[1] = v2p[src_vreg]

            # Skip over line
            linenum += 1

            if len(curr_vregs[1]) == 1 and isinstance(curr_vregs[1][0], VRegister):
                dst_reg = curr_vregs[1][0]
                # Check if we need to store output
                if not isinstance(v2p[dst_reg], Register) and isinstance(dst_reg, VRegister) and not line.opcode.startswith("store"):
                    line.dst[0] = self.pregs[2]
                    # Store
                    insert = Program.spillreg(v2p[dst_reg], self.pregs[2], self.pregs[1])
                    for i in insert:
                        self.instrs.insert(linenum, i)
                        linenum += 1
                else:
                    line.dst[0] = v2p[dst_reg]

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
