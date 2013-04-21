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

	int *base;
	char line[1000];

	/*Take care of size*/
	fgets(line, 100, file);
	base = get_size(line);

	/*read lines and place data*/
	read_lines(file, base);

}

int *get_size(char line[1000])
{
	int i = 0;
	int j = 0;
	int c = line[0];
	int total_size;
	char size[10];
	int *base;

	/*Get to char in string where size is defined*/
	while(c < '0' || c > '9')
	{
		i++;
		c =  line[i];
	}

	/*Populate size*/
	while(c != '\n' && c != '\0')
	{
		size[j] = line[i];
		i++;
		j++;
		c = line[i];
	}

	/*Convert to int*/
	total_size = (int) strtol(size, NULL, 16);

	base = malloc(total_size);

	return base;

}

void read_lines(FILE *file, int *base)
{
	char line[1000];
	char keyword[10];
	char *instrs;
	int c;
	int i;


	c = fgetc(file);

	while(c != EOF)
	{
		i = 0;
		/*Grab the line*/
		fgets(line, 1000, file);
		c = fgetc(file);
		while(line[i] != ' ' && line[i] != '\t')
		{
			keyword[i] = line[i];
			i++;
		}
		if(strcmp(keyword, "byte") == 0)
		{
			parse_byte(line, base);
		}
		else if(strcmp(keyword, "string") == 0)
		{
			parse_string(line, base);
		}
		else if(strcmp(keyword, "long") == 0)
		{
			parse_long(line, base);
		}
		else if(strcmp(keyword, "bss") == 0)
		{
			parse_bss(line, base);
		}
		else if(strcmp(keyword, "text") == 0)
		{
			instrs = strdup(line);
			printf("Current line is %s\n", instrs);
		}
		else
		{
			break;
		}
		memset(line, 0, 100);
		memset(keyword, 0, 10);
	}

}

/*BEGIN FILE PARSING FUNCTIONS*/

void parse_byte(char line[1000], int *base)
{
	printf("Current line is %s\n", line);
}

void parse_string(char line[1000], int *base)
{
	printf("Current line is %s\n", line);
}

void parse_long(char line[1000], int *base)
{
	printf("Current line is %s\n", line);
}

void parse_bss(char line[1000], int *base)
{
	printf("Current line is %s\n", line);
}

/*END FILE PARSING FUNCTIONS*/

void print_help()
{
	printf("Usage: ./y86emul [-h] <y86 input file>\n");
}

/*-----------------------------------------------------------------------------/
/--Duplicate a string and return a pointer to the duplicate--------------------/
/-----------------------------------------------------------------------------*/
char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup)
    {
        strcpy(dup, str);
    }
    return dup;
}
