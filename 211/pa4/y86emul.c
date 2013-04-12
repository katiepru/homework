#include "y86emul.h"

int main(int argc, char *argv[])
{
	FILE *file;

	/*Check for right number of args*/
	if(argc != 2)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments.\n");
		print_help();
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

	run_program(file);

	return 0;
}

void run_program(FILE *file)
{
	int c;
	int i = 0;
	char *word;

	word = calloc(1000, sizeof(char));

	c = fgetc(file);

	while(c != EOF)
	{
		if(i > 999)
		{
			fprintf(stderr, "Fuck you\n");
			return;
		}

		if(c == '\n')
		{
			parse_line(word);
		}
		else
		{
			word[i] = i;
		}
		i++;
		c = fgetc(file);
	}

}

void parse_line(char *line)
{
	char instr[8];
	int c = line[0];
	int i = 0;

	while(c != ' ' && c != '\t')
	{
		instr[i] = line[i];
		i++;
	}

	line[i] = '\0';

	/*get to rest of shit*/
	while(line[i] == ' ' || line[i] == '\t')
	{
		i++;
	}

	if(strcmp(instr, ".size") == 0)
	{
		size_instr(line, i);
	}
	else if(strcmp(instr, ".string") == 0)
	{
		string_instr(line, i);
	}
	else if(strcmp(instr, ".long") == 0)
	{
		long_instr(line, i);
	}
	else if(strcmp(instr, ".byte") == 0)
	{
		byte_instr(line, i);
	}
	else if(strcmp(instr, ".text") == 0)
	{
		text_instr(line, i);
	}
	else if(strcmp(instr, ".bss") == 0)
	{
		bss_instr(line, i);	
	}
	else
	{
		fprintf(stderr, "Bad instr %s\n", instr);
	}
}
void print_help()
{
	printf("Usage: ./y86emul [-h] <y86 input file>\n");
}
