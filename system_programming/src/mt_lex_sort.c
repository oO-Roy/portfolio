/* 
	Developer: Roy Steiner.
	File: mt_lex_sort.
	Reviewer: 
	Date:
	Status: Approved.
*/
#include <stdlib.h> /*malloc & free*/
#include <pthread.h> /*typedef, create, join*/
#include <string.h> /*strcmp*/

#include "mt_lex_sort.h" 

#define SUCCESS 0
#define FAIL -1

typedef struct thr_data
{
	char** section_start;
    size_t section_size;
}thr_data_t;

int CompareFunc(const void* data1, const void* data2)
{
	return (strcmp(*(const char**) data1, *(const char**) data2));
}

static void* ThreadWork(void* param)
{
	size_t size = 0;
	char** arr = NULL;

	arr = ((thr_data_t*)param)->section_start;
    size = ((thr_data_t*)param)->section_size;
	qsort(arr, size, sizeof(char*), CompareFunc);
	
	return NULL;
}

static int CreateNRunThreads(char** words, size_t size, pthread_t* threads
					, thr_data_t* params, size_t num_threads, size_t residues)
{
    size_t i = 0;

    for (i = 0; i < num_threads; ++i)
    {
        params[i].section_start = (words + (i * size));
        params[i].section_size = size;
        if ((num_threads - 1) == i && residues)
        {
            params[i].section_size += residues;
        }

        if (pthread_create(&threads[i], NULL, ThreadWork, &params[i]))
        {
            return FAIL;
        }
    }

    for (i = 0; i < num_threads; ++i)
    {
        if (pthread_join(threads[i], NULL))
        {
            return FAIL;
        }
    }

    return SUCCESS;
}

static void MergeSort(char* words[], size_t size, size_t num_threads
                                                        , size_t section_size)
{
    size_t i = 0;
    size_t j = 0;
    size_t min_index = 0;
    size_t section_end = 0;

    char* min_str = NULL;
    char** buffer = NULL;
    size_t* indices = NULL;

    buffer = malloc(sizeof(char*) * size);
    if (NULL == buffer)
    {
        return;
    }
    
    indices = malloc(sizeof(size_t) * num_threads);
    if (NULL == indices)
    {
        free(buffer);
        return;
    }
    
    for (i = 0; i < num_threads; ++i)
    {
        indices[i] = i * section_size;
    }
    
    for (i = 0; i < size; ++i)
    {
        min_index = -1;
        min_str = NULL;

        for (j = 0; j < num_threads; ++j)
        {
            section_end = ((j + 1) * section_size);
            if ((num_threads - 1) == j)
            {
                section_end = size;
            }

            if (indices[j] < section_end)
            {
                if (NULL == min_str || 0 > strcmp(words[indices[j]], min_str))
                {
                    min_str = words[indices[j]];
                    min_index = j;
                }
            }
        }

        if (-1 != (int)min_index)
        {
            buffer[i] = min_str;
            indices[min_index]++;
        }
    }
    
    memcpy(words, buffer, size * sizeof(char*));

    free(buffer);
    free(indices);
}

int MTLexographicSort(const char* words[], size_t size, size_t num_threads)
{
	size_t residues = 0;
	size_t section_size = 0;
	pthread_t* threads = NULL;
	thr_data_t* params = NULL;

	threads = malloc(sizeof(pthread_t) * num_threads);
	if (NULL == threads)
	{
		return FAIL;
	}

	params = malloc(sizeof(thr_data_t) * num_threads);
    if (NULL == params)
    {
        free(threads);
        return FAIL;
    }
	
	residues = (size % num_threads);
	section_size = (size / num_threads);

	if (CreateNRunThreads((char**)words, section_size, threads, params
													, num_threads, residues))
    {
        return FAIL;
    }   

	MergeSort((char**)words, size, num_threads, section_size);

	return SUCCESS;
}


