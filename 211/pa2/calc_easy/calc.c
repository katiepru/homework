#include "calc.h"

int main(int argc, char *argv[])
{
	char *result;
	int res;
	char c;
	int i;
	int nums[2];
	
	/*Check for correct number of args*/
	if(argc != 5)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments.\n");
		return 2;
	}

	/*Convert input numbers to decimal*/
	for(i = 0; i < 2; i++)
	{
		c = argv[i + 2][0];
		if(c == '-')
		{
			c = argv[i + 2][1];
		}

		/*Check input format and convert everything to decimal*/
		switch(c)
		{
			case 'd':
				nums[i] = str_to_dec(argv[i + 2]);
				break;
			case 'b':
				nums[i] = bin_to_dec(argv[i + 2]);
				break;
			case 'o':
				nums[i] = oct_to_dec(argv[i + 2]);
				break;
			case 'x':
				nums[i] = hex_to_dec(argv[i + 2]);
			default:
				fprintf(stderr, "ERROR: Unrecognized format.\n");
				return 3;
		}
	}

	c = argv[1][0];

	/*Check operand*/
	switch(c)
	{
		case '+':
			res = nums[0] + nums[1];
			break;
		case '-':
			res = nums[0] + nums[1];
			break;
		case '*':
			res = nums[0] + nums[1];
			break;
		default:
			fprintf(stderr, "ERROR: Unrecognized operation.\n");
			return 4;
	}

	c = argv[5][0];

	/*Check output format and convert to string*/
	switch(c)
	{
		case 'd':
			result = dec_to_str(res);
			break;
		case 'b':
			result = dec_to_bin(res);
			break;
		case 'o':
			result = dec_to_oct(res);
			break;
		case 'x':
			result = dec_to_hex(res);
			break;
		default:
			fprintf(stderr, "ERROR: Unrecognized output format.\n");
	}

	printf("%s\n", result);

	return 0;
}
