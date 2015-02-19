from reg_alloc.instruction import Instruction
from reg_alloc.register import Register, PRegister, VRegister

class Program:

    def __init__(self, filename):
        self.instrs = []
        self.vregs = []
        self.pregs = []

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
        for i in range(0, 3):
            self.feas.append(PRegister(i))
        raise NotImplementedError()

    def bottom_up(self, numregs):
        for i in range(0, 2):
            self.feas.append(PRegister(i))
        raise NotImplementedError()

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
