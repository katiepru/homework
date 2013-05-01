#include "y86dis.h"

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
		print_help();
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
	disassemble(file);

	return 0;
}

void disassemble(FILE *file)
{
	char line[1000];
	long addr;
	char instrs[1000];
	char directive[10];
	char byte[3];
	char val[9];
	int i = 0;
	int j, tmp;
	
	memset(instrs, 0, 1000);
	memset(byte, 0, 3);
	while(fgets(line, 1000, file) != NULL)
	{
		sscanf(line, "%s", directive);
		if(strcmp(directive, ".text") != 0)
		{
			continue;
		}
		sscanf(line, "%s %lx %s", directive, &addr, instrs);
		printf("Instructions start at offset 0x%lx.\n", addr);
		while(i < strlen(instrs))
		{
			byte[0] = instrs[i];
			byte[1] = instrs[i+1];
			i+=2;

			if(strcmp(byte, "00") == 0)
			{
				//noop
				printf("noop\n");
			}
			else if(strcmp(byte, "10") == 0)
			{
				//halt
				printf("halt\n");
			}
			else if(strcmp(byte, "20") == 0)
			{
				//rrmovl
				printf("rrmovl %s %s\n", get_reg(instrs[i]), 
					get_reg(instrs[i+1]));
				i += 2;
			}
			else if(strcmp(byte, "30") == 0)
			{
				//irmovl
				tmp = i+2;
				for(j = 0; j < 9; j++)
				{
					val[j] = instrs[tmp];
					tmp++;
				}
				printf("irmovl %s 0x%s\n", get_reg(instrs[i+1]), val);
				i += 10;
			}
		}
	}
}

const char *get_reg(char c)
{
	switch(c)
	{
		case '0':
			return "\%eax";
		case '1':
			return "\%ecx";
		case '2':
			return "\%edx";
		case '3':
			return "\%ebx";
		case '4':
			return "\%esp";
		case '5':
			return "\%ebp";
		case '6':
			return "\%esi";
		case '7':
			return "\%edi";
		default:
			return "Error";
	}
}

void print_help()
{
	printf("Usage: y86dis [-h] <y86 input file>\n");
}
