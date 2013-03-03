for i in range(0, 16):
	buf = 4 - len(bin(i)[2:])
	string = ""
	while buf > 0:
		string += "0"
		buf -= 1
	string += str(bin(i)[2:])
	print "else if(strcmp(quad, \"" + string + "\") == 0);"
	print "{"
	print "\tresult[res_ind] = '" + hex(i)[2:] + "';"
	print "}"
