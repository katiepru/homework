#include <stdio.h>
#include <stdlib.h>

int factorial(int n);
int nCr(int n, int r);

int main(int argc, char *argv[])
{
	int num;

	if(argc != 3)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments\n");
		return 2;
	}

	num = nCr((atoi(argv[1])), (atoi(argv[2])));

	printf("%d\n", num);

	return 0;
}

int nCr(int n, int r)
{
	float result;
	int numerator;
	int denominator;

	numerator = factorial(n);
	if(numerator == 0 || numerator == -1)
		return 0;
	
	denominator = factorial(r)*factorial(n-r);

	if(denominator <= 0)
		return 0;

	result = ((float) numerator)/((float) denominator);

	return result;
	
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
