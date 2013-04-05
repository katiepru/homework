#include "formula.h"

int main(int argc, char *argv[])
{
	int num;

	if(argc < 2)
	{
		fprintf(stderr, "ERROR: Too few arguments.\n");
		print_help();
		return 1;
	}
	if(argc > 2)
	{
		fprintf(stderr, "ERROR: Too many arguments.\n");
		print_help();
		return 1;
	}
	if(strcmp(argv[1], "-h") == 0)
	{
		print_help();
		return 0;
	}

	num = atoi(argv[1]);
	if(num < 0)
	{
		fprintf(stderr, "ERROR: Negative integer detected.\n");
		print_help();
		return 2;
	}

	binomial(num);

	return 0;
}

void binomial(int num)
{
	char **terms = calloc(num+1, sizeof(char *));
	char pow[11];
	char str[25];
	char *ptr;
	int ncr;
	int i;

	terms[0] = "1";

	for(i = 1; i < num+1; i++)
	{
		ncr = nCr(num, i);
		sprintf(str, "%d", nCr);
		sprintf(pow, "%d", i);
		if(i == 1)
		{
			strcat(str, "*x");
		}
		else
		{
			strcat(str, "*x^");
			strcat(str, pow);
		}
		
		/*Insert into array*/
		*ptr = str;
		terms[i] = strdup(ptr);

		/*Reset strings*/
		memset(pow, 0, strlen(pow));
		memset(str, 0, strlen(str));
	}

	for(i = 0; i < num; i++)
	{
		printf("%s + ", terms[i]); 
	}
	printf("%s\n", terms[i]);
	free(terms);
}

void print_help()
{
	printf("Usage: formula <positive integer>\n");
}

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
