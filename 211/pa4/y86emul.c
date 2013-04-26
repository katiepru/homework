#include "y86emul.h"

int main(int argc, char *argv[])
{
	FILE *file;

	/*Check for right number of args*/
	if(argc != 2)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments.\n");
	//	print_help();
		return 1;
	}

	/*Check for help mode*/
	if(strcmp(argv[1], "-h") == 0)
	{
	//	print_help();
		return 0;
	}

	/*Open the file*/
	file = fopen(argv[1], "r");

	/*Check if file was found*/
	if(file == 0)
	{
		fprintf(stderr, "ERROR: File %s not found.\n", argv[1]);
		return 2;
	}

	run_program(file);

	return 0;
}

/*Start parsing the file*/
void run_program(FILE *file)
{
	void *base;
	char line[100];

	/*Take care of size*/
	fgets(line, 100, file);	
	base = get_size(line);

	/*read lines and place data*/
	//read_lines(file, base);

}

void *get_size(char line[100])
{
	int size = 0;
	void * base;
	int stat;
	
	printf("line is %s\n", line);
	stat = sscanf(line, "%x", &size);

	printf("stat is %d, size is %x\n", stat, size);

	base = malloc(size);

	return (void *) base;

}

void put_long(long *addr, long num)
{
	memcpy(addr, &num, 4);
}

long get_long(long *addr)
{
	long num;

	memcpy(&num, addr, 4);
	return num;
}
