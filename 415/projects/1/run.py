#!/usr/bin/env python
from reg_alloc import run
import sys
if len(sys.argv) != 4:
    print """Usage: ./run <numregs> <algo> <filename>\n
           "Where algo is 0 for simple top down, 1 for
           live range top down, and 2 for bottum up"""
run(sys.argv[1], sys.argv[2], sys.argv[3])
