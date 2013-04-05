#include <stdio.h>
#include <stdlib.h>

int compute_fib(int n, int *cache);

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

	num = atoi(argv[1]);

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
		return 0;
	}

	for(i = 0; i < 47; i++)
	{
		cache[i] = -1;
	}

	num = compute_fib(num, cache);

	printf("Value: %d\n", num);


	return 0;
}

int compute_fib(int n, int *cache)
{
	int num1;
	int num2;
	int sum;

	if(cache[n] != -1)
	{
		return cache[n];
	}

	if(n == 0)
	{
		cache[0] = 0;
		return 0;
	}

	if(n == 1)
	{
		cache[1] = 1;
		return 1;
	}

	num1 = compute_fib(n-1, cache);
	num2 = compute_fib(n-2, cache);
	sum = num1 + num2;

	cache[n] = sum;

	return sum;


}
