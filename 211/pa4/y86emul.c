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
	char line[200];

	/*Take care of size*/
	fgets(line, 200, file);
	base = get_size(line);

	/*read lines and place data*/
	read_lines(FILE *file);

}

int *get_size(char line[200])
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

void read_lines(FILE *file)
{
	char line[200];
	int c;

	c = fgetc(file);

	while(c != EOF)
	{
		/*Grab the line*/
		fgets(line, 200, file);
		c = fgetc(file);


	}

}

void print_help()
{
	printf("Usage: ./y86emul [-h] <y86 input file>\n");
}
