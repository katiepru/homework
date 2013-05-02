#include "y86dis.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *file;
	int i;

	/*Check for right number of args*/
	if(argc != 2)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments.\n");
		/*print_help();*/
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

	/*this will hold the list of function locations*/
	int address_list[1000];

	memset(address_list, -1, 1000);

	find_functions(file, address_list);

	rewind(file);

	disassemble(file, address_list);

}

void find_functions(FILE *file, int* address_list)
{
	int i = 0;
	int j = 0;
	int address_counter = 0;
	unsigned int addr;
	char val[8];
	char byte[3];
	char line[1000];
	char instrs[1000];
	char directive[10];

	memset(byte, 0, 3);

	while(fgets(line, 1000, file) != NULL)
	{
		sscanf(line, "%s", directive);
		if(strcmp(directive, ".text"))
		{
			continue;
		}

		sscanf(line, "%s %x %s", directive, &addr, instrs);

		while(i < strlen(instrs))
		{
			byte[0] = instrs[i];
			byte[1] = instrs[i+1];
			i+=2;

			if(!strcmp(byte, "80"))
			{
				/*call*/
				for(j = 0; j < 8; j+=2)
				{
					val[7-j] = instrs[i+1];
					val[7-(j+1)] = instrs[i];
					i+=2;
				}
				val[j] = '\0';

				/*grab the address to jump to as a number*/
				addr = strtol(val, NULL, 16);

				for(j = 0; j < address_counter; j++ )
				{
					if(address_list[j] == addr)
					{
						break;
					}
				}

				if(j >= address_counter)
				{
					address_list[address_counter] = addr;
					address_counter++;
				}
			}
		}
	}
}

void disassemble(FILE *file, int* address_list)
{
	char line[1000];
	unsigned int addr;
	char instrs[1000];
	char directive[10];
	char byte[3];
	char val[8];
	int i = 0;
	int j, tmp, func_addr;

	memset(instrs, 0, 1000);
	memset(byte, 0, 3);
	while(fgets(line, 1000, file) != NULL)
	{
		sscanf(line, "%s", directive);
		if(strcmp(directive, ".text") != 0)
		{
			continue;
		}
		sscanf(line, "%s %x %s", directive, &addr, instrs);
		printf("instrs are %s\n", instrs);
		printf("Instructions start at offset 0x%x.\n", addr);
		while(i < strlen(instrs))
		{
			for(j=0; j < 1000; j++)
			{
				if(address_list[j] == (i/2) + addr)
				{
					printf(".function%d\n", j);
					break;
				}
				if(address_list[j] == -1)
				{
					break;
				}
			}

			byte[0] = instrs[i];
			byte[1] = instrs[i+1];
			i+=2;

			if(strcmp(byte, "00") == 0)
			{
				/*noop*/
				printf("    noop\n");
			}
			else if(strcmp(byte, "10") == 0)
			{
				/*halt*/
				printf("    halt\n");
			}
			else if(strcmp(byte, "20") == 0)
			{
				/*rrmovl*/
				printf("    rrmovl %s %s\n", get_reg(instrs[i]),
					get_reg(instrs[i+1]));
				i += 2;
			}
			else if(strcmp(byte, "30") == 0)
			{
				/*irmovl*/
				tmp = i+2;
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[tmp];
					tmp++;
				}
				val[j] = '\0';
				printf("    irmovl %s 0x%s\n", get_reg(instrs[i+1]), val);
				i += 10;
			}
			else if(strcmp(byte, "40") == 0)
			{
				/*rmmovl*/
				tmp = i+2;
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[tmp];
					tmp++;
				}
				val[j] = '\0';
				printf("    rmmovl %s 0x%s(%s)\n", get_reg(instrs[i]), val,
					get_reg(instrs[i+1]));
			}
			else if(strcmp(byte, "50") == 0)
			{
				/*mrmovl*/
				tmp = i+2;
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[tmp];
					tmp++;
				}
				val[j] = '\0';
				printf("    mrmovl 0x%s(%s) %s\n", val, get_reg(instrs[i+1]),
					get_reg(instrs[i]));
			}
			else if(strcmp(byte, "60") == 0)
			{
				/*addl*/
				printf("    addl %s %s\n", get_reg(instrs[i]),
					get_reg(instrs[i+1]));
				i += 2;
			}
			else if(strcmp(byte, "61") == 0)
			{
				/*subl*/
				printf("    subl %s %s\n", get_reg(instrs[i]),
					get_reg(instrs[i+1]));
				i += 2;
			}
			else if(strcmp(byte, "62") == 0)
			{
				/*andl*/
				printf("    andl %s %s\n", get_reg(instrs[i]),
					get_reg(instrs[i+1]));
				i += 2;
			}
			else if(strcmp(byte, "63") == 0)
			{
				/*xorl*/
				printf("    xorl %s %s\n", get_reg(instrs[i]),
					get_reg(instrs[i+1]));
				i += 2;
			}
			else if(strcmp(byte, "64") == 0)
			{
				/*mull*/
				printf("    mull %s %s\n", get_reg(instrs[i]),
					get_reg(instrs[i+1]));
				i += 2;
			}
			else if(strcmp(byte, "70") == 0)
			{
				/*jmp*/
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[i];
					i++;
				}
				val[j] = '\0';
				printf("    jmp 0x%s\n", val);
			}
			else if(strcmp(byte, "71") == 0)
			{
				/*jle*/
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[i];
					i++;
				}
				val[j] = '\0';
				printf("    jle 0x%s\n", val);
			}
			else if(strcmp(byte, "72") == 0)
			{
				/*jl*/
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[i];
					i++;
				}
				val[j] = '\0';
				printf("    jl 0x%s\n", val);
			}
			else if(strcmp(byte, "73") == 0)
			{
				/*je*/
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[i];
					i++;
				}
				val[j] = '\0';
				printf("    je 0x%s\n", val);
			}
			else if(strcmp(byte, "74") == 0)
			{
				/*jne*/
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[i];
					i++;
				}
				val[j] = '\0';
				printf("    jne 0x%s\n", val);
			}
			else if(strcmp(byte, "75") == 0)
			{
				/*jge*/
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[i];
					i++;
				}
				val[j] = '\0';
				printf("    jge 0x%s\n", val);
			}
			else if(strcmp(byte, "80") == 0)
			{
				/*call*/
				for(j = 0; j < 8; j+=2)
				{
					val[7-j] = instrs[i+1];
					val[7-(j+1)] = instrs[i];
					i+=2;
				}
				val[j] = '\0';
				func_addr = strtol(val, NULL, 16);

				for(j=0; j < 1000; j++)
				{
					if( address_list[j] == func_addr )
					{
						printf("    call .%s%d\n", "function", j);
						break;
					}
					else if ( func_addr == -1 )
					{
						/*failsafe*/
						printf("    call 0x%s\n", val);
						break;
					}
				}

				if ( j >= 1000 )
				{
					/*failsafe*/
					printf("    call 0x%s\n", val);
				}
			}
			else if(strcmp(byte, "a0") == 0)
			{
				/*pushl*/
				printf("    pushl %s\n", get_reg(instrs[i]));
				i += 2;
			}
			else if(strcmp(byte, "b0") == 0)
			{
				/*popl*/
				printf("    popl %s\n", get_reg(instrs[i]));
				i += 2;
			}
			else if(strcmp(byte, "c0") == 0)
			{
				/*readb*/
				tmp = i+2;
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[tmp];
					tmp++;
				}
				val[j] = '\0';
				printf("    readb 0x%s(%s)\n", val, get_reg(instrs[i]));
				i += 10;
			}
			else if(strcmp(byte, "c1") == 0)
			{
				/*readw*/
				tmp = i+2;
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[tmp];
					tmp++;
				}
				val[j] = '\0';
				printf("    readw 0x%s(%s)\n", val, get_reg(instrs[i]));
				i += 10;
			}
			else if(strcmp(byte, "d0") == 0)
			{
				/*writeb*/
				tmp = i+2;
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[tmp];
					tmp++;
				}
				val[j] = '\0';
				printf("    writeb 0x%s(%s)\n", val, get_reg(instrs[i]));
				i += 10;
			}
			else if(strcmp(byte, "d1") == 0)
			{
				/*writew*/
				tmp = i+2;
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[tmp];
					tmp++;
				}
				val[j] = '\0';
				printf("    writew 0x%s(%s)\n", val, get_reg(instrs[i]));
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
			printf("Error %c\n", c);
			return "";
	}
}

void print_help()
{
	printf("Usage: y86dis [-h] <y86 input file>\n");
}
