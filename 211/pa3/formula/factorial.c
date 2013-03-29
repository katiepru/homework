#include <stdio.h>
#include <stdlib.h>

int factorial(int n);

int main(int argc, char *argv[])
{
	int num;

	if(argc != 2)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments\n");
		return 2;
	}

	num = factorial(atoi(argv[1]));

	printf("%d\n", num);

	return 0;
}

int factorial(int n)
{
	int product = 1;
	int prev_product = 1;
	int overflow = 0;

	if(n == 0) return 1;

	if(n<0) return -1;

	while(n>0)
	{
		product *= n;
		if(product < prev_product)
		{
			overflow = 1;
			break;
		}
		prev_product = product;
		n--;
	}

	if(overflow) return 0;
	
	return product;
}
