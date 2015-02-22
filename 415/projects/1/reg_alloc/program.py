from reg_alloc.instruction import Instruction
from reg_alloc.register import Register, PRegister, VRegister

class Program:

    def __init__(self, filename):
        self.instrs = []
        self.vregs = []
        self.pregs = []
        self.spilloff = -4

        with open(filename, "r") as f:
            linenum = 1
            for line in f:
                if line.startswith("//") or line.isspace():
                    continue
                i = Instruction(line, linenum, self.vregs)
                self.instrs.append(i)
                linenum += 1

    def allocate_registers(self, algo, numregs):
        if numregs > len(self.regs):
            return self.ret_original()
        if algo == 0:
            return self.top_down1(numregs)
        elif algo == 1:
            return self.top_down2(numregs)
        elif algo == 2:
            return self.bottum_up(numregs)
        else:
            raise ValueError("Invalid algorithm %d" % algo)

    def top_down1(self, numregs):
        feasible_set = 3
        for i in range(0, numregs):
            self.pregs.append(PRegister(i))

        self.vregs.sort(reverse=True, key=lambda r: r.occurances)

        # Add loading the base register
        load_base = "loadI 1024 => %s" % str(self.pregs[0])
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
            live_regs[i] = []
            for reg in self.vregs:
                if i in range(reg.live_range[0], reg.live_range[1] + 1):
                    live_regs[i].push(reg)

        live_regs.sort(reverse=True, key=lambda l: len(l))
        v2p = {}
        spilloff = -4

        for line in live_regs:
            if len(line) <= numregs - feasible_set:
                break
            while not_spilled(line, v2p) > numregs - feasible_set:
                # Need to spill. Pick reg with most occurances
                # If tied, pick longest live range
                spillreg = VRegister(-1)
                for reg in line:
                    if reg in v2p:
                        continue
                    if reg.occurances > spillreg.occurance:
                        spillreg = reg
                    elif reg.occurances == spillreg.occurances:
                        if reg.live_range[1] - reg.live_range[0] > spillreg.live_range[1] - spillreg.live_range[0]:
                            spillreg = reg
                v2p[spillreg] = spilloff
                spilloff -= 4

        # Allocate registers
        self.gen_code_from_mapping(v2p)

    def bottom_up(self, numregs):
        feasible_set = 3
        for i in range(0, numregs):
            self.pregs.append(PRegister(i))

        # Add loading the base register
        load_base = "loadI 1024 => %s" % str(self.pregs[0])
        self.instrs.insert(0, Instruction(load_base, 0, [], isphys=True))

        linenum = 1
        orig_linenum = 0
        while linenum < len(self.instrs):
            line = self.instrs[linenum]
            curr_vregs = line.get_regs()
            i = 1
            spillcode = []
            for reg in curr_vregs[0] + curr_vregs[1]:
                if isinstance(reg, PRegister):
                    continue
                if reg.spilloff >= 0:
                    if reg.preg is None:
                        preg = self.get_available_preg(orig_linenum)
                        if preg is None:
                            farthest_use = self.get_farthest_preg(orig_linenum)
                            print "spilling %d for line %s" % (farthest_use.vreg.num, str(line))
                            # Spill farthest
                            spillcode += Program.spillreg(self.spilloff,
                                                      farthest_use,
                                                      self.pregs[1])
                            for s in spillcode:
                                self.instrs.insert(linenum, s)
                                linenum += 1
                            reg.preg = farthest_use
                            reg.preg.vreg = reg
                            farthest_use.vreg.spilloff = self.spilloff
                            self.spilloff -= 4
                        else:
                            reg.preg = preg
                            preg.vreg = reg

                if reg.spilloff < 0 and (reg in curr_vregs[0] or "store" in line.opcode):
                    print "Loading %d spilloff %d for line %s" % (reg.num,
                            reg.spilloff, str(line))
                    # Load spilled register
                    insert = Program.loadreg(reg.spilloff, self.pregs[i])
                    reg.preg = self.pregs[i]
                    i += 1
                    for ins in insert:
                        self.instrs.insert(linenum, ins)
                        linenum += 1

                # Replace vreg with preg
                if line.src[0] == reg:
                    line.src[0] = reg.preg
                elif len(line.src) > 1 and line.src[1] == reg:
                    line.src[1] = reg.preg
                else:
                    line.dst[0] = reg.preg

            linenum += 1
            orig_linenum += 1

    def gen_code_from_mapping(self, v2p):
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
                if not isinstance(v2p[dst_reg], Register) and isinstance(dst_reg, VRegister):
                    line.dst[0] = self.pregs[2]
                    # Store
                    insert = Program.spillreg(v2p[dst_reg], self.pregs[2], self.pregs[1])
                    for i in insert:
                        self.instrs.insert(linenum, i)
                        linenum += 1
                else:
                    line.dst[0] = v2p[dst_reg]

    def get_farthest_preg(self, linenum):
        farthest = self.pregs[3]
        for reg in self.pregs[3:]:
            if not reg.vreg:
                continue
            if reg.vreg.uses[0] <= linenum:
                reg.vreg.uses.pop(0)
            if len(reg.vreg.uses) == 0:
                return reg
            if reg.vreg.uses[0] > farthest.vreg.uses[0]:
                farthest = reg
        return farthest

    def get_available_preg(self, linenum):
        for reg in self.pregs[3:]:
            if not reg.vreg or len(reg.vreg.uses) == 0:
                return reg
        return None


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
