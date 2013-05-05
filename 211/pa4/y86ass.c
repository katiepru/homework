#include "y86ass.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *file;
	int i;
	unsigned int offset;
	/*this will hold the list of function locations*/
	int function_list[1000];
	/*and this will hold the jumps*/
	int jump_list[1000];

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

	memset(function_list, -1, 1000);
	memset(jump_list, -1, 1000);

	pre_assemble(file, function_list, jump_list);

	rewind(file);

	assemble(file, function_list, jump_list);
}

/*Print anything not on the .text line and locate functions and jumps in memory*/
void pre_assemble(FILE *file, int* function_list, int* jump_list)
{
	int tmp;
	unsigned int offset;
	unsigned int current_position = 0;
	char val[12];
	char line[1000];
	char directive[12];

	while(fgets(line, 1000, file) != NULL)
	{
		sscanf(line, "%s", directive);
		if(!strcmp(directive, ".string") ||
		   !strcmp(directive, ".long")   ||
		   !strcmp(directive, ".size")   ||
		   !strcmp(directive, ".byte"))
		{
			printf(line);
		}
		else if(!strcmp(directive, ".text"))
		{
			sscanf(line, "%s %s", directive, val);
			offset = strtol(val, NULL, 16);
		}
		else if(!strncmp(directive, ".function", 9))
		{
			tmp = atoi(&directive[9]);
			function_list[tmp] = current_position + offset;
		}
		else if(!strncmp(directive, ".L", 2)){
			tmp = atoi(&directive[2]);
			jump_list[tmp] = current_position + offset;
		}
		else if(!strcmp(directive, "halt") ||
				!strcmp(directive, "noop") ||
				!strcmp(directive, "ret"))
		{
			current_position += 1;
		}
		else if(!strcmp(directive, "rrmovl") ||
				!strcmp(directive, "pushl") ||
				!strcmp(directive, "popl") ||
				!strcmp(directive, "addl") ||
				!strcmp(directive, "subl") ||
				!strcmp(directive, "andl") ||
				!strcmp(directive, "xorl") ||
				!strcmp(directive, "mull"))
		{
			current_position += 2;
		}
		else if(!strcmp(directive, "call") ||
				directive[0] == 'j')
		{
			current_position += 5;
		}
		else if(!strcmp(&directive[2], "movl") ||
				!strncmp(directive, "read", 4) ||
				!strncmp(directive, "write", 5))
		{
			current_position += 6;
		}
	}
	printf(".text\t%x\t", offset);
}

void assemble(FILE *file, int* function_list, int* jump_list)
{
	int tmp;
	char line[1000];
	char directive[20];
	char arg1[20];
	char arg2[20];

	while(fgets(line, 1000, file) != NULL)
	{
		sscanf(line, "%s %s %s", directive, arg1, arg2);

		/*fprintf(stderr, "\n\nline: %s\n", line);*/
		/*fprintf(stderr, "directive: %s\narg1: %s\narg2: %s\n", directive, arg1, arg2);*/

		if(!strcmp(directive, "noop"))
		{
			printf("00");
		}
		else if(!strcmp(directive,"halt"))
		{
			printf("10");
		}
		else if(!strcmp(directive,"rrmovl"))
		{
			/*fprintf(stderr, "rrmovl\n");*/
			printf("20");
			printf("%c", get_register(arg1));
			printf("%c", get_register(arg2));
		}
		else if(!strcmp(directive,"irmovl"))
		{
			/*fprintf(stderr, "irmovl\n");*/
			printf("30f");
			printf("%c", get_register(arg2));
			printf("%s", &arg1[2]);
		}
		else if(!strcmp(directive,"rmmovl"))
		{
			/*fprintf(stderr, "rmmovl\n");*/
			printf("40");
			printf("%c", get_register(arg1));
			printf("%c", get_register(&arg2[11]));
			printf("%.8s", &arg2[2]);
		}
		else if(!strcmp(directive,"mrmovl"))
		{
			/*fprintf(stderr, "mrmovl\n");*/
			printf("50");
			printf("%c", get_register(arg2));
			printf("%c", get_register(&arg1[11]));
			printf("%.8s", &arg1[2]);
		}
		else if(!strcmp(directive, "addl") ||
				!strcmp(directive, "subl") ||
				!strcmp(directive, "andl") ||
				!strcmp(directive, "xorl") ||
				!strcmp(directive, "mull"))
		{
			printf("%c", '6');
			if      ( !strcmp ( directive, "addl" ) ) { printf ( "%c", '0' ) ; }
			else if ( !strcmp ( directive, "subl" ) ) { printf ( "%c", '1' ) ; }
			else if ( !strcmp ( directive, "andl" ) ) { printf ( "%c", '2' ) ; }
			else if ( !strcmp ( directive, "xorl" ) ) { printf ( "%c", '3' ) ; }
			else if ( !strcmp ( directive, "mull" ) ) { printf ( "%c", '4' ) ; }

			/*fprintf(stderr, "op1\n");*/
			printf("%c", get_register(arg1));
			printf("%c", get_register(arg2));
		}
		else if(directive[0] == 'j')
		{
			printf("%c", '7');
			if      ( !strcmp ( directive, "jmp" )  ) { printf ( "%c", '0' ) ;}
			else if ( !strcmp ( directive, "jle" )  ) { printf ( "%c", '1' ) ;}
			else if ( !strcmp ( directive, "jl"  )  ) { printf ( "%c", '2' ) ;}
			else if ( !strcmp ( directive, "je"  )  ) { printf ( "%c", '3' ) ;}
			else if ( !strcmp ( directive, "jne" )  ) { printf ( "%c", '4' ) ;}
			else if ( !strcmp ( directive, "jge" )  ) { printf ( "%c", '5' ) ;}
			tmp = atoi(&arg1[2]);
			/*printf("%x", jump_list[tmp]);*/
			print_as_little_endian(jump_list[tmp]);
		}
		else if(!strcmp(directive, "call"))
		{
			printf("80");
			/*function*/
			tmp = atoi(&arg1[9]);
			print_as_little_endian(function_list[tmp]);
		}
		else if(!strcmp(directive, "ret"))
		{
			printf("90");
		}
		else if(!strcmp(directive, "pushl"))
		{
			/*fprintf(stderr, "pushl\n");*/
			printf("a0");
			printf("%cf", get_register(arg1));
		}
		else if(!strcmp(directive, "popl"))
		{
			/*fprintf(stderr, "popl\n");*/
			printf("b0");
			printf("%cf", get_register(arg1));
		}
		else if(!strncmp(directive, "read", 4))
		{
			/*fprintf(stderr, "read\n");*/
			printf("%c", 'c');
			if(directive[4] == 'b'){ printf("%c", '0'); }
			else                   { printf("%c", '1'); }
			printf("%cf", get_register(&arg1[11]));
			printf("%.8s", &arg1[2]);
		}
		else if(!strncmp(directive, "write", 5))
		{
			/*fprintf(stderr, "write\n");*/
			printf("%c", 'd');
			if(directive[5] == 'b'){ printf("%c", '0'); }
			else                   { printf("%c", '1'); }
			printf("%cf", get_register(&arg1[11]));
			printf("%.8s", &arg1[2]);
		}

	}
	printf("\n");
}

void print_help()
{
	printf("Usage: y86dis [-h] <y86 input file>\n");
}

char get_register(char* reg)
{
	/*fprintf(stderr, "got %s\n", reg);*/
	if(!strncmp(reg, "%eax", 4))
	{
		/*fprintf(stderr, "matched 0\n", reg);*/
		return '0';
	}
	else if(!strncmp(reg, "%ecx", 4))
	{
		/*fprintf(stderr, "matched 1\n", reg);*/
		return '1';
	}
	else if(!strncmp(reg, "%edx", 4))
	{
		/*fprintf(stderr, "matched 2\n", reg);*/
		return '2';
	}
	else if(!strncmp(reg, "%ebx", 4))
	{
		/*fprintf(stderr, "matched 3\n", reg);*/
		return '3';
	}
	else if(!strncmp(reg, "%esp", 4))
	{
		/*fprintf(stderr, "matched 4\n", reg);*/
		return '4';
	}
	else if(!strncmp(reg, "%ebp", 4))
	{
		/*fprintf(stderr, "matched 5\n", reg);*/
		return '5';
	}
	else if(!strncmp(reg, "%esi", 4))
	{
		/*fprintf(stderr, "matched 6\n", reg);*/
		return '6';
	}
	else if(!strncmp(reg, "%edi", 4))
	{
		/*fprintf(stderr, "matched 7\n", reg);*/
		return '7';
	}
	/*fprintf(stderr, "missed it\n");*/
	return '!';
}

void print_as_little_endian(int num)
{
	int i;
	int converted;
	char as_string[9];
	as_string[8] = '\0';
	converted = ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
	sprintf(as_string, "%x", converted);
	for(i=0; i < 8 - strlen(as_string); i++)
	{
		printf("0");
	}
	printf("%s", as_string);
}
