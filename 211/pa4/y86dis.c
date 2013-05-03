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
	int function_list[1000];
	/*and this will hold the jumps*/
	int jump_list[1000];

	memset(function_list, -1, 1000);
	memset(jump_list, -1, 1000);

	find_functions_and_jumps(file, function_list, jump_list);

	/*for(i=0; i< 1000; i++)                      */
	/*{                                           */
	/*    if(jump_list[i] != -1)                  */
	/*    {                                       */
	/*        printf("%d: %d\n", i, jump_list[i]);*/
	/*    }                                       */
	/*    else                                    */
	/*    {                                       */
	/*        break;                              */
	/*    }                                       */
	/*}                                           */

	rewind(file);

	disassemble(file, function_list, jump_list);
}

void find_functions_and_jumps(FILE *file, int* function_list, int* jump_list)
{
	int i = 0;
	int j = 0;
	int function_counter = 0;
	int jump_counter = 0;
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
			printf(line);
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

				for(j = 0; j < function_counter; j++ )
				{
					if(function_list[j] == addr)
					{
						break;
					}
				}

				if(j >= function_counter)
				{
					function_list[function_counter] = addr;
					function_counter++;
				}
			}
			else if(!(
						strcmp(byte, "70") &&
						strcmp(byte, "71") &&
						strcmp(byte, "72") &&
						strcmp(byte, "73") &&
						strcmp(byte, "74") &&
						strcmp(byte, "75")
					))
			{
				/*jump of some sort*/
				strncpy(val, &instrs[i], 8);
				val[8] = '\0';

				/*printf("Copied string: %s\n", val);*/

				/*grab the address to jump to as a number*/
				addr = to_big_endian_int(val);

				/*printf("returned address: %d\n", addr);*/

				for(j=0; j < jump_counter; j++)
				{
					if( jump_list[j] ==  addr)
					{
						break;
					}
				}

				if(j >= jump_counter)
				{
					jump_list[jump_counter] = addr;
					jump_counter++;
				}
			}
			else if(byte[0] == '2' ||
					byte[0] == '6' ||
					byte[0] == 'a' ||
					byte[0] == 'b')
			{
				i += 2;
			}
			else if(byte[0] == '3' ||
					byte[0] == '4' ||
					byte[0] == '5' ||
					byte[0] == 'c' ||
					byte[0] == 'd')
			{
				i += 10;
			}
			else if(byte[0] == '7' ||
					byte[0] == '8')
			{
				i += 8;
			}
		}
	}
}

void disassemble(FILE *file, int* function_list, int* jump_list)
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
		printf("%s\t%x\n", directive, addr);
		while(i < strlen(instrs))
		{
			for(j=0; j < 1000; j++)
			{
				if(function_list[j] == (i/2) + addr)
				{
					printf(".function%d\n", j);
					break;
				}
				if(function_list[j] == -1)
				{
					break;
				}
			}

			for(j=0; j < 1000; j++)
			{
				if(jump_list[j] == (i/2) + addr)
				{
					printf(".L%d\n", j);
					break;
				}
				if(function_list[j] == -1)
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
				strncpy(val, &instrs[i+2], 8);
				val[8] = '\0';
				printf("    irmovl %s 0x%s\n", get_reg(instrs[i+1]), val);
				i += 10;
			}
			else if(strcmp(byte, "40") == 0)
			{
				/*rmmovl*/
				strncpy(val, &instrs[i+2], 8);
				val[8] = '\0';
				printf("    rmmovl %s 0x%s(%s)\n", get_reg(instrs[i]), val,
					get_reg(instrs[i+1]));
				i+=10;
			}
			else if(strcmp(byte, "50") == 0)
			{
				/*mrmovl*/
				strncpy(val, &instrs[i+2], 8);
				val[8] = '\0';
				printf("    mrmovl 0x%s(%s) %s\n", val, get_reg(instrs[i+1]),
					get_reg(instrs[i]));
				i+=10;
			}
			else if(byte[0] == '6')
			{
				/*op1*/
				printf("    %s %s %s\n", math_ops[byte[1] - '0'], get_reg(instrs[i]),
					get_reg(instrs[i+1]));
				i += 2;
			}
			else if(byte[0] == '7')
			{
				/*j<X>*/
				strncpy(val, &instrs[i], 8);
				val[8] = '\0';
				i+=8;
				tmp = to_big_endian_int(val);
				tmp = check_for_jump(tmp, jump_list);
				if(tmp != -1)
				{
					printf("    %s .L%d\n", jump_ops[byte[1] - '0'], tmp);
				}
				else
				{
				printf("    %s 0x%s\n", jump_ops[byte[1] - 1], val);
				}
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
					if( function_list[j] == func_addr )
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
			else if(strcmp(byte, "90") == 0)
			{
				/*ret*/
				printf("    ret\n");

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
			else if(byte[0] == 'c')
			{
				/*read<X>*/
				tmp = i+2;
				for(j = 0; j < 8; j++)
				{
					val[j] = instrs[tmp];
					tmp++;
				}
				val[j] = '\0';
				printf("    %s 0x%s(%s)\n", read_ops[byte[1] - '0'], val, get_reg(instrs[i]));
				i += 10;
			}
			else if(byte[0] == 'd')
			{
				/*write<X>*/
				strncpy(val, &instrs[i+2], 8);
				val[8] = '\0';
				printf("    %s 0x%s(%s)\n", write_ops[byte[1] - '0'], val, get_reg(instrs[i]));
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

int check_for_jump(int addr, int* jump_list)
{
	/*printf("\nChecking addr %d\n\n", addr);*/
	int i;
	for(i=0; i< 1000; i++)
	{
		if(jump_list[i] == addr)
		{
			return i;
		}
		if(jump_list[i] == -1)
		{
			return -1;
		}
	}
	return -1;
}

int to_big_endian_int(char* little_endy_str)
{
	int string_lenght = strlen(little_endy_str);
	/*printf("String length: %d\n", string_lenght);*/
	char new_str[string_lenght+1];
	int i;

	for(i=0; i<string_lenght; i+=2)
	{
		new_str[(string_lenght-i)-1] = little_endy_str[i+1];
		new_str[(string_lenght-(i+1))-1] = little_endy_str[i];
	}

	new_str[i] = '\0';

	/*printf("New string: %s\n", new_str);*/

	return strtol(new_str, NULL, 16);
}
