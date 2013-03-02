for i in range(0, 16):
	print "else if(strcmp(quad, \"" + bin(i)[2:]+ "\") == 0);"
	print "{"
	print "\tresult[res_ind] = '" + hex(i)[2:] + "';"
	print "}"
