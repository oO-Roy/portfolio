#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "mt_countingsort.h"

static int IsCharValid(char ch)
{
	return (((('a' <= ch) && ('z' >= ch)) || (('A' <= ch) && ('Z' >= ch))) 
												&& ('\n' != ch) && '\0' != ch);
}

static size_t NumOfCharsInDictionary(void)
{
	size_t num = 0;
	char ch = 0;
	FILE *dictionary = NULL;

	dictionary = fopen("/usr/share/dict/words", "r");
	if (NULL == dictionary)
	{
		return 1;
	}

	for (; EOF != ch;)
	{
		ch = fgetc(dictionary);
		num += IsCharValid(ch);
	}
	
	fclose(dictionary);

	return num;
}

int main(void)
{
	size_t i = 0;
	size_t size = NumOfCharsInDictionary();

	FILE *dictionary = NULL;
	char* dict_arr = NULL;
	char ch = 0;

	int start = 0;
	int end = 0;

	dict_arr = malloc(size);
	if (NULL == dict_arr)
	{
		return 1;
	}
	
	dictionary = fopen("/usr/share/dict/words", "r");
	if (NULL == dictionary)
	{
		return 1;
	}

	for (; EOF != ch && i < size; ++i)
	{
		ch = fgetc(dictionary);
		
		if (IsCharValid(ch))
		{
			dict_arr[i] = ch;
		}
	}

	fclose(dictionary);

	start = clock();
	MTCountingSort(dict_arr, size, 12);
	end = clock();

	for (i = 0; i < size; ++i)
    {
        printf("%c", dict_arr[i]);
    } 

	free(dict_arr);

	printf("\nTime of sorting: %d\n", (end - start));
	

	return 0;
}
