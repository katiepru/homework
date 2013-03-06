for i in range(1, 16):
	string = bin(i)[2:]
	while len(string) < 4:
		string = "0" + string
	print "case '" + hex(i)[2:] + "':"
	print "\tresult[bin_ind] = '" + string[3] + "';"
	print "\tresult[bin_ind-1] = '" + string[2] + "';"
	print "\tresult[bin_ind-2] = '" + string[1] + "';"
	print "\tresult[bin_ind-3] = '" + string[0] + "';"
	print"\tbreak;"
