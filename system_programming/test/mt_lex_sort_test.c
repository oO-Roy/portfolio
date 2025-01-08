#include <stdlib.h> /*malloc & free*/
#include <stdio.h> /*FILES, Dictionary, stdin*/
#include <string.h> /*strlen*/
#include <time.h> /*clock_t, clock*/

#include "mt_lex_sort.h"

#define UNUSED(x) (void)(x)
#define DICTIONARY_PATH "/usr/share/dict/words"
#define MULTIPLE 4
#define NUM_OF_THREADS 1

int CompFunc(const void* data1, const void* data2)
{
	return rand() * (strlen((char*)data1) + rand() % *(int*)data2);
}

static void* ReadDictNFillBuffer(size_t* num_of_letters, size_t* num_of_words)
{
	size_t i = 0;
	
	FILE *dictionary = NULL;
	char* buffer = NULL;
	char* b_runner = NULL;

	dictionary = fopen(DICTIONARY_PATH, "r");
	if (NULL == dictionary)
	{
		return NULL;
	}

	fseek(dictionary, 0, SEEK_END);	
	*num_of_letters = ftell(dictionary);
	fseek(dictionary, 0, SEEK_SET);

	buffer = malloc(sizeof(char) * (*num_of_letters) * MULTIPLE);
	if (NULL == buffer)
	{
		return NULL;
	}

	b_runner = buffer;

	while (EOF != (*b_runner = fgetc(dictionary)))
	{
		if ('\n' == *b_runner)
		{
			*b_runner = '\0';
			++(*num_of_words);
		}
		
		++b_runner;
	}

	fclose(dictionary);

	for (i = 1; i < MULTIPLE; ++i)
	{
		memcpy((buffer + (*num_of_letters * i)), buffer, *num_of_letters);
	}

	return buffer;
}

int main(void)
{
	size_t i = 0;
	size_t j = 0;

	size_t num_of_letters = 0;
	size_t num_of_words = 0;
	char* buffer = NULL;
	const char** words = NULL;

	__clock_t start = 0; 
	__clock_t end = 0; 

	buffer = ReadDictNFillBuffer(&num_of_letters, &num_of_words);

	words = malloc(sizeof(char*) * num_of_words * MULTIPLE);
	if (NULL == words)
	{
		return -1;
	}
	
	words[0] = &buffer[0];
	for (i = 1, j = 1; i < (num_of_letters * MULTIPLE - 1); ++i)
	{
		if ('\0' == buffer[i])
		{
			words[j++] = &buffer[i + 1];
		}
	}

	qsort(words, (MULTIPLE * num_of_words - 1), sizeof(char*), CompFunc);

	start = clock();
	MTLexographicSort(words, (num_of_words * MULTIPLE), NUM_OF_THREADS);
	end = clock();

	/* for (i = 0; i < (num_of_words * MULTIPLE); ++i)
	{
		printf("%s ", words[i]);
	}*/

	printf("%ld\n", (end-start));

	free(buffer);
	free(words);

	return 0;
}