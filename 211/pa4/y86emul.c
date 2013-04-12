#include "y86emul.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments.\n");
		print_help();
		return 1;
	}
	return 0;
}

void print_help()
{
	printf("Usage: ./y86emul [-h] <y86 input file>\n");
}
