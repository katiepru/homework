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

void read_lines(FILE *file, void *base)
{
	char line[1000];
	char directive[10];
	char *instrs;
	char *bss[2];
	char byte;
	char str[1000];
	char *str_ptr;
	long num;
	long addr_offset;
	void *addr;
	int i;

	while(fgets(line, 1000, file) != NULL)
	{
		sscanf(line, "%s", directive);
		if(strcmp(directive, ".long") == 0)
		{
			sscanf(line, "%s %lx %d", directive, &addr, &num);
			addr = (void *)((long) base + addr_offset);
			put_long((long *) addr, num);
		}
		else if(strcmp(directive, ".byte") == 0)
		{
			sscanf(line, "%s %lx %x", directive, &addr_offset, &byte);
			addr = (char *) ((long) base + addr_offset);
			put_byte(addr, byte);
		}
		else if(strcmp(directive, ".string") == 0)
		{
			sscanf(line, "%s %lx %s", directive, &addr_offset, str);
			addr = (char *) ((long) base + addr_offset);
			str_ptr = calloc(strlen(str), sizeof(char));
			for(i = 0; i <= strlen(str); i++)
			{
				str_ptr[i] = str[i];
			}
			put_string(addr, str_ptr);
		}
		else if(strcmp(directive, ".bss") == 0)
		{
		}
		else
		{
			fprintf(stderr, "ERROR: Invalid directive %s.\n", directive);
			return;
		}
	}
}

/*Helper functions to get and put data to and from memory*/

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

void put_byte(char *addr, char num)
{
	memcpy(addr, &num, 1);
}

char get_byte(char *addr)
{
	char num;

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
