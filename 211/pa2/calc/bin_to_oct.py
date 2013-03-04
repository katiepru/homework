for i in range(0, 8):
	buf = 3 - len(bin(i)[2:])
	string = ""
	while buf > 0:
		string += "0"
		buf -= 1
	string += str(bin(i)[2:])
	print "else if(strcmp(trio, \"" + string + "\") == 0)"
	print "{"
	print "\tresult[res_ind] = '" + oct(i)[1:] + "';"
	print "}"
