#include <stdio.h>
#include <stdlib.h>

int fib(int n);

int main(int argc, char *argv[])
{
	int num;

	if(argc != 2)
	{
		fprintf(stderr, "Wrong number of args\n");
		return 2;
	}

	num = fib(atoi(argv[1]));

	printf("%d\n", num);

	return 0;
}

int fib(int n)
{

}
