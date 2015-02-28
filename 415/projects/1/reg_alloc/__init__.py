from reg_alloc.program import Program
import sys


version_tuple = (0, 1)

def run(regs, algo, filename):
    try:
        int(regs)
    except:
        print "Number of registers should be an integer"
        sys.exit(1)

    p = Program(filename)
    p.allocate_registers(algo, regs)

    for i in p.instrs:
        print str(i)
