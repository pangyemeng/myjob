#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "html_util.h"

int transfer_mean(char *str)
{
	char *head = NULL, *end = NULL;
	if (str == NULL)
	{
		return 0;
	}
	head = str;
	end = head;
	while (*head != '\0')
	{
		if ((*head == '\\\\'))
		{
			*head = '\\';
		}
		if (end < head)
		{
			*end = *head;
		}
		end++;
		head++;
	}
	*end = '\0';
	return 1;
}

int html_decode(char *src, char *dest)
{
	int strlength = strlen(src);
	if (strlength < 3)
	{
		strcpy(dest, src);
		return 0;
	}
	int i = 0;
	int flag = 0;
	int j = 0;
	char tmp1 = 0;
	char tmp2 = 0;
	char tmpA = 0;
	char tmpB = 0;
	char A, B;

	for (i = 0; i < strlength; i++)
	{
		if (src[i] == '%')
		{
			flag = 1;
			continue;
		}
		switch (flag)
		{
		case 0:
		{

			dest[j] = src[i];
			if (dest[j] == '+')
				dest[j] = ' ';
			j++;
			break;
		}
		case 1:
			flag = 2;
			if (i < (strlength - 12) && src[i] == '5' && src[i + 1] == 'C'
					&& src[i + 2] == '%' && src[i + 3] == '5' && src[i + 4]
					== 'C' && src[i + 5] == 'r' && src[i + 6] == '%' && src[i
					+ 7] == '5' && src[i + 8] == 'C' && src[i + 9] == '%'
					&& src[i + 10] == '5' && src[i + 11] == 'C' && src[i + 12]
					== 'n')
			{
				dest[j] = 10;
				i = i + 12;
				flag = 0;
				j++;
				break;
			}
			if (i < (strlength - 20) && src[i] == '5' && src[i + 1] == 'C'
					&& src[i + 2] == '%' && src[i + 3] == '2' && src[i + 4]
					== '2' && src[i + 5] == '%' && src[i + 6] == '2' && src[i
					+ 7] == 'C' && src[i + 8] == '%' && src[i + 9] == '5'
					&& src[i + 10] == 'C' && src[i + 11] == '%' && src[i + 12]
					== '2' && src[i + 13] == '2' && src[i + 14] == '%' && src[i
					+ 15] == '5' && src[i + 16] == 'C' && src[i + 17] == '%'
					&& src[i + 18] == '5' && src[i + 19] == 'C' && src[i + 20]
					== 'n')
			{
				dest[j] = 10;
				i = i + 20;
				flag = 0;
				j++;
				break;
			}
			if (i < (strlength - 10) && src[i] == '5' && src[i + 1] == 'C'
					&& src[i + 2] == '%' && src[i + 3] == '5' && src[i + 4]
					== 'C' && src[i + 5] == '%' && src[i + 6] == '5' && src[i
					+ 7] == 'C' && src[i + 8] == '%' && src[i + 9] == '5'
					&& src[i + 10] == 'C')
			{
				dest[j] = '\\';
				i = i + 10;
				flag = 0;
				j++;
				break;
			}
			tmpA = src[i];

			break;
		case 2:
			tmpB = src[i];
			tmp1 = toupper(tmpA);
			tmp2 = toupper(tmpB);
			if (((tmp1 >= 48 && tmp1 <= 57) || (tmp1 >= 65 && tmp1 <= 90))
					&& ((tmp2 >= 48 && tmp2 <= 57)
							|| (tmp2 >= 65 && tmp2 <= 90)))
			{
				if (tmp1 >= 48 && tmp1 <= 57)
					A = tmp1 - 48;
				else
					A = 10 + tmp1 - 65;
				if (tmp2 >= 48 && tmp2 <= 57)
					B = tmp2 - 48;
				else
					B = 10 + tmp2 - 65;
				dest[j] = A * 16 + B;

			}
			else
			{
				dest[j] = '%';
				dest[j + 1] = tmp1;
				dest[j + 2] = tmp2;
				j += 2;
			}
			flag = 0;
			j++;
			break;
		default:
			break;
		}
	}
	dest[j] = 0;
	printf("len = %d, %s\n", j, dest);
	return 1;
}
