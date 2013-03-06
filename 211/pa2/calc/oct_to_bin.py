for i in range(1, 8):
	string = bin(i)[2:]
	while len(string) < 3:
		string = "0" + string
	print "case '" + str(i) + "':"
	print "\tresult[bin_ind] = '" + string[2] + "';"
	print "\tresult[bin_ind-1] = '" + string[1] + "';"
	print "\tresult[bin_ind-2] = '" + string[0] + "';"
	print"\tbreak;"
