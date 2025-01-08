#include <stdio.h>
#include <pthread.h>
#include <time.h>   /*time_t, time()*/
#include <unistd.h> /*sleep()*/
#include <omp.h>

#define SIZE 100000
#define INPUT 10000000000
#define NUM_OF_THREADS 12
int arr[SIZE] = {0};

typedef struct thread_calc
{
    size_t start_index;
    size_t end_index;
    size_t thread_result;
    size_t num;
} thread_calc_t;

static void *FillArray(void *index)
{
    arr[(size_t)index] = (size_t)index;

    return NULL;
}

int EX1_3(void)
{
    size_t i = 0;
    int status = 0;
    
    pthread_t thread_id[SIZE] = {0};
    pthread_attr_t attr = {0};

    time_t start = 0;
    time_t end = 0;

    start = time(0);

    for (i = 0; i < SIZE; ++i)
    {
       pthread_attr_init(&attr);
       pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); 
       status = pthread_create(&thread_id[i], &attr, &FillArray, (size_t*)i);

       while (0 != status)
       {
            status = pthread_create(&thread_id[i], &attr, &FillArray
                                                                , (size_t*)i);
       }

       pthread_attr_destroy(&attr);
    }

    /*without using the 'detach' attribute - we use the join*/
    /* for (i = 0; i < SIZE; ++i)
    {
        pthread_join(thread_id[i], NULL);
    } */

    end = time(0);

    for (i = 0; i < SIZE; ++i)
    {
        if (arr[i] != (int)i)
        {
            printf("Array data ERROR\n");
            break;
        }
    }

    printf("difftime: %.3f[seconds]\n", difftime(end, start));

    return 0;
}

static void *CheckForDivisorsInRange(void *thread_data)
{
    size_t i = 0;
    size_t sum_of_divisors = 0;

    for (i = ((thread_calc_t *)thread_data)->start_index; 
                        i <= ((thread_calc_t *)thread_data)->end_index; ++i)
    {
        if (0 == ((thread_calc_t *)thread_data)->num % i)
        {
            sum_of_divisors += i;
        }
    }

    return (void*)sum_of_divisors;
}

void InitThreadData(thread_calc_t thread_data[], size_t num_of_threads)
{
    size_t i = 0;
    size_t section_length = (INPUT / num_of_threads);

    thread_data[0].num = INPUT;
    thread_data[0].start_index = 1;
    thread_data[0].end_index = section_length;

    for (i = 1; i < num_of_threads; ++i)
    {
        thread_data[i].num = INPUT;
        thread_data[i].start_index = thread_data[(i - 1)].end_index + 1;
        thread_data[i].end_index = 
                                thread_data[i].start_index + section_length;
    }
}

size_t FinalResult(thread_calc_t thread_data[])
{
    size_t i = 0;
    size_t final_result = 0;

    for (; i < NUM_OF_THREADS; ++i)
    {
        final_result += thread_data[i].thread_result;
    }

    return final_result;
}

int EX4(void)
{
    size_t i = 0;
    int status = 0;

    pthread_t thread_id[NUM_OF_THREADS] = {0};
    thread_calc_t thread_data[NUM_OF_THREADS] = {0};

    InitThreadData(thread_data, NUM_OF_THREADS);

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        status = pthread_create(&thread_id[i], NULL, &CheckForDivisorsInRange
                                                        , &thread_data[i]);
        while (0 != status)
        {
            status = pthread_create(&thread_id[i], NULL
                                , &CheckForDivisorsInRange, &thread_data[i]);
        }
    }

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        status = pthread_join(thread_id[i]
                                    , (void*)&(thread_data[i].thread_result));

        while (0 != status)
        {
            status = pthread_join(thread_id[i]
                                    ,  (void*)&(thread_data[i].thread_result));
        }
    }

    printf("Result: %ld\n", FinalResult(thread_data));

    return 0;
}

int EX5_6(void)
{
    size_t i = 0;
    size_t final_result = 0;

    #pragma omp parallel for
    for (i = 1; i <= INPUT; ++i)
    {
        if (0 == INPUT % i)
        {
            final_result += i;
        }
        
    }

    printf("Result: %ld\n", final_result);

    return 0;
}