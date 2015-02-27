from reg_alloc.program import Program


version_tuple = (0, 1)

def run(regs, algo, filename):
    p = Program(filename)
    p.allocate_registers(algo, regs)

    for i in p.instrs:
        print str(i)
