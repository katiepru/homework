#include "calc.h"

int main(int argc, char *argv[])
{
	char *result;

	result = oct_to_bin(argv[1]);
	printf("bin is %s\n", result);

	result = bin_to_oct(result);
	printf("oct is %s\n", result);

	return 0;
}

char *oct_to_bin(char *num)
{
	char *result;
	int oct_ind = strlen(num)-1;
	int bin_ind;

	if(num[0] == '-')
	{
		result = calloc(((strlen(num)-2)*3)+1, sizeof(char));
		bin_ind = (strlen(num)-2)*3;
	}
	else
	{
		result = calloc(((strlen(num)-1)*3)+1, sizeof(char));
		bin_ind = (strlen(num)-1)*3;
	}

	for(bin_ind = bin_ind; bin_ind >= 2; bin_ind -= 3)
	{
		switch(num[oct_ind])
		{
			case '0':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				break;
			case '1':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				break;
			case '2':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				break;
			case '3':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				break;
			case '4':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				break;
			case '5':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				break;
			case '6':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				break;
			case '7':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				break;
			default:
				fprintf(stderr, "ERROR: Bad octal char.\n");
		}
		oct_ind--;
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

char *bin_to_oct(char *num)
{
	char *result;
	char *triplet = calloc(3, sizeof(char));
	int bin_ind = strlen(num)-1;
	int res_ind;
	int brk = 0;

	if(num[0] == '-')
	{
		result = calloc((strlen(num)-1)/3, sizeof(char));
		res_ind = (strlen(num)/3)+1;
	}
	else
	{
		result = calloc((strlen(num)/3), sizeof(char));
		res_ind = (strlen(num)/3)+2;
	}
	
	while(bin_ind >= 2)
	{
		triplet[0] = num[bin_ind-2];
		triplet[1] = num[bin_ind-1];
		triplet[2] = num[bin_ind];

		if(strcmp(triplet, "000") == 0)
		{
			result[res_ind] = '0';
		}
		else if(strcmp(triplet, "001") == 0)
		{
			result[res_ind] = '1';
		}
		else if(strcmp(triplet, "010") == 0)
		{
			result[res_ind] = '2';
		}
		else if(strcmp(triplet, "011") == 0)
		{
			result[res_ind] = '3';
		}
		else if(strcmp(triplet, "100") == 0)
		{
			result[res_ind] = '4';
		}
		else if(strcmp(triplet, "101") == 0)
		{
			result[res_ind] = '5';
		}
		else if(strcmp(triplet, "110") == 0)
		{
			result[res_ind] = '6';
		}
		else if(strcmp(triplet, "111") == 0)
		{
			result[res_ind] = '7';
		}
		else if(triplet[0] == '-')
		{
			brk = 1;
			break;
		}
		else
		{
			fprintf(stderr, "Bad octal triplet\n");
		}
		res_ind--;
		bin_ind -= 3;
		memset(triplet, 0, sizeof(triplet));
	}
	if(bin_ind == 1 || brk)
	{
		if(num[0] == '-')
		{
			if(num[1] == '0')
			{
				result[res_ind] = '0';
			}
			else
			{
				result[res_ind] = '1';
			}
			result[res_ind-1] = '-';
			res_ind -= 2;
		}
		else
		{
			if(num[0] == '0' && num[1] == '0')
			{
				result[res_ind] = '0';
			}
			else if(num[0] == '0' && num[1] == '1')
			{
				result[res_ind] = '1';
			}
			else if(num[0] == '1' && num[1] == '0')
			{
				result[res_ind] = '2';
			}
			else
			{
				result[res_ind] = '3';
			}
			res_ind--;
		}
	}

	else if(bin_ind == 0)
	{
		result[res_ind] = num[0];
		res_ind--;
	}

	while(res_ind >= 0)
	{
		result[res_ind] = '0';
		res_ind--;
	}

	free(triplet);

	result = strip_zeroes(result);

	free(num);

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
