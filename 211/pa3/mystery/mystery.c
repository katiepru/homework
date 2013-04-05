#include <stdio.h>
#include <stdlib.h>

int compute_fib(int n);

int main(int argc, char *argv[])
{
	int cache[47];
	int i;
	int num;

	if(argc <= 2)
	{
		if(argc == 1)
		{
			fprintf(stderr, 
				"Error. no argument provided! Syntax: ./myster >pos_int>\n");
			return 0;
		}
	}
	else
	{
		fprintf(stderr, 
			"ERROR. Too many arguments! Syntax: ./mystery <pos_int>\n");
		return 0;
	}

	if(!num)
	{
		fprintf(stderr, 
			"ERROR. Negative Integer! Syntax: ./mystery <pos_int>\n");
		return 0;
	}

	if(num > 46)
	{
		fprintf(stderr,
			"ERROR. Integer too large! Syntax: ./mystery <pos_int>\n");
	}

	for(i = 0; i < 47; i++)
	{
		cache[i] = -1;
	}

	num = compute_fib(num);

	printf("Value: %d\n", num);


	return 0;
}

int compute_fib(int n)
{

}
