#include <stdlib.h> /*malloc & free*/
#include <pthread.h> /*pthread_create,*/
#include <assert.h> /*assert*/

#include "mt_countingsort.h" /*typedef, size_t*/

#define UNUSED(x) (void)(x)
#define NUMS_OF_CHAR 256
#define KB 1024
#define OFFSET 128

static size_t g_LUT[NUMS_OF_CHAR];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
size_t g_residues;

typedef enum status /*described in API*/
{
    SUCCESS,
    ALLOC_FAIL,
    SYS_FAIL,
    ARGS_INVALID
}status_t;

typedef struct thread_data
{
    char* section_start;
    size_t section_size;
}thread_data_t;

/*----------------------------------------------------------------------------*/
static void* ThreadWork(void* param)
{
    size_t i = 0;
    size_t size = 0;
    size_t LUT[NUMS_OF_CHAR] = {0};

    char* a_runner = NULL;
    char* PADDING[KB]; /* just in case :) */
    
    UNUSED(PADDING);

    a_runner = ((thread_data_t*)param)->section_start;
    size = ((thread_data_t*)param)->section_size;

    for (; i < size; ++i, ++a_runner)
    {
        ++LUT[((int)*a_runner + OFFSET)];
    }

    pthread_mutex_lock(&mutex);
    for (i = 0; i < NUMS_OF_CHAR; ++i)
    {
        g_LUT[i] += LUT[i];
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

static status_t CreateNRunThreads(char* arr, size_t size
        , pthread_t* threads_ids, thread_data_t* params, size_t num_threads)
{
    size_t i = 0;

    for (i = 0; i < num_threads; ++i)
    {
        params[i].section_start = (arr + (size * i));
        params[i].section_size = size;
        if ((num_threads - 1) == i)
        {
            params[i].section_size += g_residues;
        }

        if (pthread_create(&threads_ids[i], NULL, ThreadWork, &params[i]))
        {
            return SYS_FAIL;
        }
    }

    for (i = 0; i < num_threads; ++i)
    {
        if (pthread_join(threads_ids[i], NULL))
        {
            return SYS_FAIL;
        }
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
int MTCountingSort(char* arr, size_t size, size_t num_threads)
{
    size_t i = 0;
    size_t j = 0;

    size_t section_size = 0;
    pthread_t* threads_ids = NULL;
    thread_data_t* thread_params = NULL;

    assert(arr);

    threads_ids = malloc(sizeof(pthread_t) * num_threads);
    if (NULL == threads_ids)
    {
        return ALLOC_FAIL;
    }

    thread_params = malloc(sizeof(thread_data_t) * num_threads);
    if (NULL == thread_params)
    {
        free(threads_ids);
        return ALLOC_FAIL;
    }

    g_residues = (size % num_threads);
    section_size = (size / num_threads);

    if (CreateNRunThreads(arr, section_size, threads_ids, thread_params
                                                                , num_threads))
    {
        return SYS_FAIL;
    }                

    for (i = 0; i < size && j < NUMS_OF_CHAR; ++j)
    {
        for (; g_LUT[j] > 0 ; ++i, --g_LUT[j])
        {
            arr[i] = (char)(j - OFFSET);
        } 
    }
    
    pthread_mutex_destroy(&mutex);
    free(threads_ids);
    free(thread_params);

    return SUCCESS;
}



