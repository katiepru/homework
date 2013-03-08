#include "calc.h"

int main(int argc, char *argv[])
{
	char *result;

	result = hex_to_bin(argv[1]);
	printf("bin is %s\n", result);

	result = bin_to_hex(result);
	printf("hex is %s\n", result);

	return 0;
}

char *hex_to_bin(char *num)
{
	char *result; 
	int hex_ind = strlen(num)-1;
	int bin_ind;

	if(num[0] == '-')
	{
		result = calloc(((strlen(num)-2)*4)+1, sizeof(char));
		bin_ind = ((strlen(num)-2)*4);
	}
	else
	{
		result = calloc(((strlen(num)-1)*4)+1, sizeof(char));
		bin_ind = ((strlen(num)-1)*4)-1;
	}

	for(bin_ind = bin_ind; bin_ind >= 3; bin_ind -= 4)
	{
		switch(num[hex_ind])
		{
			case '0':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '0';
				break;
			case '1':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '0';
				break;
			case '2':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '0';
				break;
			case '3':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '0';
				break;
			case '4':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '0';
				break;
			case '5':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '0';
				break;
			case '6':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '0';
				break;
			case '7':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '0';
				break;
			case '8':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '1';
				break;
			case '9':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '1';
				break;
			case 'a':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '1';
				break;
			case 'b':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '1';
				break;
			case 'c':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '1';
				break;
			case 'd':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '1';
				break;
			case 'e':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '1';
				break;
			case 'f':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '1';
				break;
			default:
				fprintf(stderr, "ERROR: Bad hex character.");
				break;
		}
		hex_ind--;
	}

	if(num[0] == '-')
	{
		result[bin_ind] = '-';
		bin_ind--;
	}
	while(bin_ind >= 0)
	{
		result[bin_ind] = '0';
		bin_ind--;
	}

	result = strip_zeroes(result);

	return result;
}

char *bin_to_hex(char *num)
{
	char *result;
	char *quad = calloc(4, sizeof(char));
	int bin_ind = strlen(num) - 1;
	int res_ind;

	if(num[0] == '-')
	{
		result = calloc((strlen(num)/4), sizeof(char));
		res_ind = (strlen(num)/4)+1;
	}
	else
	{
		result = calloc((strlen(num)-1)/4, sizeof(char));
		res_ind = (strlen(num)/4)+2;
	}

	while(bin_ind >= 3)
	{
		quad[0] = num[bin_ind-3];
		quad[1] = num[bin_ind-2];
		quad[2] = num[bin_ind-1];
		quad[3] = num[bin_ind];

		if(strcmp(quad, "0000") == 0)
		{
			result[res_ind] = '0';
		}
		else if(strcmp(quad, "0001") == 0)
		{
			result[res_ind] = '1';
		}
		else if(strcmp(quad, "0010") == 0)
		{
			result[res_ind] = '2';
		}
		else if(strcmp(quad, "0011") == 0)
		{
			result[res_ind] = '3';
		}
		else if(strcmp(quad, "0100") == 0)
		{
			result[res_ind] = '4';
		}
		else if(strcmp(quad, "0101") == 0)
		{
			result[res_ind] = '5';
		}
		else if(strcmp(quad, "0110") == 0)
		{
			result[res_ind] = '6';
		}
		else if(strcmp(quad, "0111") == 0)
		{
			result[res_ind] = '7';
		}
		else if(strcmp(quad, "1000") == 0)
		{
			result[res_ind] = '8';
		}
		else if(strcmp(quad, "1001") == 0)
		{
			result[res_ind] = '9';
		}
		else if(strcmp(quad, "1010") == 0)
		{
			result[res_ind] = 'a';
		}
		else if(strcmp(quad, "1011") == 0)
		{
			result[res_ind] = 'b';
		}
		else if(strcmp(quad, "1100") == 0)
		{
			result[res_ind] = 'c';
		}
		else if(strcmp(quad, "1101") == 0)
		{
			result[res_ind] = 'd';
		}
		else if(strcmp(quad, "1110") == 0)
		{
			result[res_ind] = 'e';
		}
		else if(strcmp(quad, "1111") == 0)
		{
			result[res_ind] = 'f';
		}
		else
		{
			fprintf(stderr, "Unrecognized quad %s\n.", quad);
		}
		res_ind--;
		bin_ind -= 4;
	}

	free(quad);

	if(bin_ind == 2)
	{
		if(result[0] == '-')
		{
			if(num[bin_ind] == '0' && num[bin_ind-1] == '0')
			{
				result[res_ind] = '0';
			}
			else if(num[bin_ind] == '1' && num[bin_ind-1] == '0')
			{
				result[res_ind] = '2';
			}
			else if(num[bin_ind] == '0' && num[bin_ind-1] == '1')
			{
				result[res_ind] = '1';
			}
			else
			{
				result[res_ind] = '3';
			}
			result[res_ind-1] = '-';
			res_ind -= 2;
		}
		else
		{
			if(num[bin_ind] == '0' && num[bin_ind-1] == '0' && 
				num[bin_ind-2] == '0')
			{
				result[res_ind] = '0';
			}
			else if(num[bin_ind] == '1' && num[bin_ind-1] == '0'
				&& num[bin_ind-2] == '0')
			{
				result[res_ind] = '1';
			}
			else if(num[bin_ind] == '0' && num[bin_ind-1] == '1'
				&& num[bin_ind-2] == '0')
			{
				result[res_ind] = '2';
			}
			else if(num[bin_ind] == '1' && num[bin_ind-1] == '1'
				&& num[bin_ind-2] == '0')
			{
				result[res_ind] = '3';
			}
			else if(num[bin_ind] == '0' && num[bin_ind-1] == '0'
				&& num[bin_ind-2] == '1')
			{
				printf("whoops ind is %d\n", res_ind);
				result[res_ind] = '4';
			}
			else if(num[bin_ind] == '1' && num[bin_ind-1] == '0'
			&& num[bin_ind-2] == '1')
			{
				result[res_ind] = '5';
			}
			else if(num[bin_ind] == '0' && num[bin_ind-1] == '1'
			&& num[bin_ind-2] == '1')
			{
				result[res_ind] = '6';
			}
			else
			{
				result[res_ind] = '7';
			}
			res_ind--;
		}
	}
	else if(bin_ind == 1)
	{
		if(result[0] == '-')
		{
			result[res_ind] = num[bin_ind];
			result[res_ind-1] = '-';
			res_ind -= 2;
		}
		else
		{
			if(num[bin_ind] == '0' && num[bin_ind-1] == '0')
			{
				result[res_ind] = '0';
			}
			else if(num[bin_ind] == '1' && num[bin_ind-1] == '0')
			{
				result[res_ind] = '2';
			}
			else if(num[bin_ind] == '0' && num[bin_ind-1] == '1')
			{
				result[res_ind] = '1';
			}
			else
			{
				result[res_ind] = '3';
			}
			res_ind--;
		}
	}
	else if(bin_ind == '0')
	{
		result[res_ind] = num[bin_ind];
		res_ind--;
	}

	while(res_ind >= 0)
	{
		result[res_ind] = '0';
		res_ind--;
	}

	result = strip_zeroes(result);

	return result;
}


char *strip_zeroes(char *num)
{
	char *res;
	int i;
	int res_ind = 0;
	int num_zeroes = 0;

	for(i = 0; i < strlen(num); i++)
	{
		if(num[i] != '0')
			break;
		num_zeroes++;
	}

	if(num_zeroes == 0)
	{
		return num;
	}

	if(num_zeroes == strlen(num))
	{
		free(num);
		return "0";
	}
	res = calloc(strlen(num)+1-num_zeroes, sizeof(char));
	for(i = num_zeroes; i<strlen(num); i++)
	{
		res[res_ind] = num[i];
		res_ind++;
	}

	free(num);

	return res;
}
