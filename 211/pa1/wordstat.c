#include "wordstat.h"
#include <stdio.h>
#include <string.h>

void print_help();
Node read_file();

int main(int argc, char *argv[])
{
	//Check that there is the correct number of args
	if(argc!=2)
	{
			printf("Wrong number of arguments");
			print_help();
			return 1;
	}

	//Check if in help mode
	const char help[] = "-h";
	if(strcmp(argv[1], help)==0)
	{
		print_help();
		return 0;
	}

	//Open the file
	FILE *file;
	file=fopen(argv[1], "r");

	//If fopen returns 0, file not found
	if(file==0)
	{
		printf("Could not find file: %s. Exiting.\n", argv[1]);
		return 1;
	}

	//File was successfully found, begin parsing file
	Node *head;
	head=read_file(file);

	return 0;
}

/*-----------------------------------------------------------------------------/
* Read file and create linked list of unique words-----------------------------/
------------------------------------------------------------------------------*/
Node read_file(FILE file)
{
	return NULL;
}

/*-----------------------------------------------------------------------------/
* Print help text
*-----------------------------------------------------------------------------*/
void print_help()
{
	printf("wordstat - a word counting program.\n \
	Usage: wordstat [-h] file\n \
		help (-h)		Display this message");
}
