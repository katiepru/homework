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
	read_lines(file, base);

}

void *get_size(char line[100])
{
	int size = 0;
	void * base;
	char trash[10];
	int stat;
	
	stat = sscanf(line, "%s %x", trash, &size);

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

void put_byte(long *addr, char num)
{
	memcpy(addr, &num, 1);
}

long get_byte(long *addr)
{
	long num;

	memcpy(&num, addr, 1);
	return num;
}

void put_string(char *addr, char *str)
{
	int i = 0;
	while(i <= strlen(str))
	{
		addr[i] = str[i];
		i++;
	}
}

char *get_string(char *addr, int len)
{
	char *str = calloc(len, sizeof(char));
	int i = 0;

	while(i <= len)
	{
		str[i] = addr[i];
	}
	return str;
}
