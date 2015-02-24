from reg_alloc.register import Register, VRegister, PRegister

class Instruction:

    def __init__(self, line, linenum, regs, isphys=False):
        self.src = []
        self.dst = []

        parts = line.split()
        self.opcode = parts[0]

        if self.opcode == "output":
            self.src.append(int(parts[1]))
            return

        #if not parts[0] in OPCODES:
        #    raise ValueError("Invalid opcode %s" % parts[0])
        if len(parts) < 4 or len(parts) > 5:
            raise ValueError("Invalid instruction '%s'" % line)

        trans = parts.index("=>")
        if trans < 0:
            raise ValueError("Missing =>")

        for i in range(1, trans):
            self.src.append(Instruction.parse_instr(parts[i], regs, linenum, isphys))

        for i in range(trans+1, len(parts)):
            self.dst.append(Instruction.parse_instr(parts[i], regs, linenum, isphys))

    def get_regs(self):
        ret = ([], [])

        for item in self.src:
            if isinstance(item, Register):
                ret[0].append(item)

        for item in self.dst:
            if isinstance(item, Register):
                ret[1].append(item)

        return ret

    def __str__(self):
        s = self.opcode + " "
        for i in self.src:
            s += str(i) + ", "
        s = s[:-2]
        s += " "
        if self.opcode == "output":
            return s
        s += "=>"
        for i in self.dst:
            s += " " + str(i) + ","
        s = s[:-1]
        return s

    @staticmethod
    def parse_instr(part, regs, linenum, isphys=False):
        if part.endswith(","):
            part = part[:-1]
        if part.startswith("r"):
            regnum = int(part[1:])
            for reg in regs:
                if reg.num == regnum:
                    if not isphys:
                        reg.live_range[1] = linenum - 1
                        reg.uses.append(linenum)
                    return reg
            if isphys:
                ret = PRegister(regnum)
            else:
                ret = VRegister(regnum)
                ret.live_range[0] = linenum
                ret.live_range[1] = linenum
                ret.uses.append(linenum)
                regs.append(ret)
        else:
            ret = int(part)
        return ret
