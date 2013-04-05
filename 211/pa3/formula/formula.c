#include "formula.h"

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "ERROR: Too few arguments.\n");
		print_help();
		return 1;
	}
	if(argc > 2)
	{
		fprintf(stderr, "ERROR: Too many arguments.\n")
		print_help();
		return 1;
	}
	if(strcmp(argv[1], "-h") == 0)
	{
		print_help();
		return 0;
	}

}

void print_help()
{
	printf("Usage: formula <positive integer>\n");
}
