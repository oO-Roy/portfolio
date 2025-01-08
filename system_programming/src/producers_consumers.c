#include <stdio.h>
#include <stddef.h>
#include <pthread.h>
#include <semaphore.h>

#include "sllist.h"
#include "cbuffer.h"
#include "fsq.h"

#define NUM_OF_THREADS_1 2
#define NUM_OF_THREADS_2 8
#define CBUFFER_CAPACITY 8
#define STR_SIZE 10
#define NUM_OF_ROUNDS 10

#define UNUSED(x) (void)(x)

#define RED "\033[0;31m"
#define B_RED "\033[1;31m"
#define GR "\033[0;32m"
#define B_GR "\033[1;32m"
#define RESET "\033[0m"

/*---EX1----------------------------------------------------------------------*/
int flag = 0;
int data = 0;

void* Consumer_1(void* param)
{
    UNUSED(param);

    while (1)
    {
        if (1 == flag)
        {
            /*critical section start*/
            data = __sync_sub_and_fetch(&data, 1);
            printf("data value after consumer operation: %d\n", data);
            /*critical section end*/

            flag = 0;
        }
    }

    return NULL;
}

void* Producer_1(void* param)
{
    UNUSED(param);

    while (1)
    {
        if (0 == flag)
        {
            /*critical section start*/
            data = __sync_add_and_fetch(&data, 2);
            printf("data value producer operation: %d\n", data);
            /*critical section end*/

            flag = 1;
        }
    }

    return NULL;
}

int EX1(void)
{
    pthread_t thread_id[NUM_OF_THREADS_1] = {0};

    pthread_create(&thread_id[0], NULL, &Producer_1, NULL);
    pthread_create(&thread_id[1], NULL, &Consumer_1, NULL);

    return 0;
}
/*---EX2----------------------------------------------------------------------*/
pthread_mutex_t sllist_mutex_ex2 = PTHREAD_MUTEX_INITIALIZER;

typedef struct sllist_params
{
    sllist_t* list;
    void* data;
} sll_param_t;

void* Consumers_2(void* param)
{
    int status = 1;

    while (status)
    {
        /*critical section start*/
        pthread_mutex_lock(&sllist_mutex_ex2);

        if (!SLLIsEmpty(((sll_param_t*)param)->list))
        {
            printf("node value after consumer: %d\n"
                , *(int *)SLLGetData(SLLBegin(((sll_param_t*)param)->list)));
            SLLRemove(SLLBegin(((sll_param_t*)param)->list));

            status = 0;
        }

        /*critical section end*/
        pthread_mutex_unlock(&sllist_mutex_ex2);
    }

    return NULL;
}

void* Producers_2(void* param)
{
    iterator_t iter = NULL;

    /*critical section start*/
    pthread_mutex_lock(&sllist_mutex_ex2);

    iter = SLLInsertBefore(SLLEnd(((sll_param_t*)param)->list)
                                                , ((sll_param_t*)param)->data);
    printf("node value after producer: %d\n", *(int*)SLLGetData(iter));

    /*critical section end*/
    pthread_mutex_unlock(&sllist_mutex_ex2);

    return NULL;
}

int EX2(void)
{
    size_t i = 0;

    sllist_t *list = SLLCreate();
    int data[NUM_OF_THREADS_2] = {0};

    pthread_t thread_produce[NUM_OF_THREADS_2] = {0};
    pthread_t thread_consume[NUM_OF_THREADS_2] = {0};
    sll_param_t parameters[NUM_OF_THREADS_2] = {0};

    for (i = 0; i < NUM_OF_THREADS_2; ++i)
    {
        data[i] = i + 1;
        parameters[i].list = list;
        parameters[i].data = &data[i];
    }

    for (i = 0; i < NUM_OF_THREADS_2; ++i)
    {
        pthread_create(&thread_produce[i], NULL, &Producers_2, &parameters[i]);
        pthread_create(&thread_consume[1], NULL, &Consumers_2, &parameters[i]);
    }

    pthread_exit(NULL);

    SLLDestroy(list);

    return 0;
}

/*---EX3----------------------------------------------------------------------*/
pthread_mutex_t sllist_mutex_ex3 = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_list_count_ex3;

void* Consumers_3(void* param)
{
    sem_wait(&sem_list_count_ex3);

    /*critical section start*/
    pthread_mutex_lock(&sllist_mutex_ex3);

    printf("node value after consumer: %d\n"
            , *(int*)SLLGetData(SLLBegin(((sll_param_t*)param)->list)));
    SLLRemove(SLLBegin(((sll_param_t*)param)->list));

    /*critical section end*/
    pthread_mutex_unlock(&sllist_mutex_ex3);

    return NULL;
}

void* Producers_3(void* param)
{
    iterator_t iter = NULL;

    /*critical section start*/
    pthread_mutex_lock(&sllist_mutex_ex3);

    iter = SLLInsertBefore(SLLEnd(((sll_param_t*)param)->list)
                                                , ((sll_param_t*)param)->data);
    printf("node value after producer: %d\n", *(int*)SLLGetData(iter));

    sem_post(&sem_list_count_ex3);

    /*critical section end*/
    pthread_mutex_unlock(&sllist_mutex_ex3);

    return NULL;
}

int EX3()
{
    size_t i = 0;

    sllist_t* list = SLLCreate();
    int data[NUM_OF_THREADS_2] = {0};

    pthread_t thread_produce[NUM_OF_THREADS_2] = {0};
    pthread_t thread_consume[NUM_OF_THREADS_2] = {0};
    sll_param_t parameters[NUM_OF_THREADS_2] = {0};

    for (i = 0; i < NUM_OF_THREADS_2; ++i)
    {
        data[i] = i + 1;
        parameters[i].list = list;
        parameters[i].data = &data[i];
    }

    sem_init(&sem_list_count_ex3, 0, 0);

    for (i = 0; i < NUM_OF_THREADS_2; ++i)
    {
        pthread_create(&thread_produce[i], NULL, &Producers_3, &parameters[i]);
        pthread_create(&thread_consume[1], NULL, &Consumers_3, &parameters[i]);
    }

    pthread_exit(NULL);

    SLLDestroy(list);

    return 0;
}

/*---EX4----------------------------------------------------------------------*/
pthread_mutex_t sllist_mutex_ex4 = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_buffer_empty_ex4;
sem_t sem_buffer_full_ex4;

typedef struct cbuffer_params
{
    cbuff_t* buffer;
    void* data;
} cbuff_param_t;

void* Consumers_4(void* param)
{
    char returned_data[STR_SIZE];

    sem_wait(&sem_buffer_empty_ex4);

    /*critical section start*/
    pthread_mutex_lock(&sllist_mutex_ex4);

    CBuffRead(((cbuff_param_t*)param)->buffer, returned_data, sizeof(int));
    printf("value after consumer: %d\n", *(int*)returned_data);

    sem_post(&sem_buffer_full_ex4);

    /*critical section end*/
    pthread_mutex_unlock(&sllist_mutex_ex4);

    return NULL;
}

void* Producers_4(void* param)
{
    sem_wait(&sem_buffer_full_ex4);

    /*critical section start*/
    pthread_mutex_lock(&sllist_mutex_ex4);

    CBuffWrite((((cbuff_param_t*)param)->buffer)
                                , ((cbuff_param_t*)param)->data, sizeof(int));
    printf("value after producer: %d\n"
                                    , *(int*)((cbuff_param_t*)param)->data);

    sem_post(&sem_buffer_empty_ex4);

    /*critical section end*/
    pthread_mutex_unlock(&sllist_mutex_ex4);

    return NULL;
}

int EX4()
{
    size_t i = 0;

    cbuff_t* buffer = CBuffCreate(CBUFFER_CAPACITY);
    int data[NUM_OF_THREADS_2] = {0};

    pthread_t thread_produce[NUM_OF_THREADS_2] = {0};
    pthread_t thread_consume[NUM_OF_THREADS_2] = {0};
    cbuff_param_t parameters[NUM_OF_THREADS_2] = {0};

    for (i = 0; i < NUM_OF_THREADS_2; ++i)
    {
        data[i] = i + 1;
        parameters[i].buffer = buffer;
        parameters[i].data = &data[i];
    }

    sem_init(&sem_buffer_empty_ex4, 0, 0);
    sem_init(&sem_buffer_full_ex4, 0, CBUFFER_CAPACITY);

    for (i = 0; i < NUM_OF_THREADS_2; ++i)
    {
        pthread_create(&thread_produce[i], NULL, &Producers_4, &parameters[i]);
        pthread_create(&thread_consume[1], NULL, &Consumers_4, &parameters[i]);
    }

    pthread_exit(NULL);

    CBuffDestroy(buffer);

    return 0;
}

/*---EX5----------------------------------------------------------------------*/
typedef struct fsq_params
{
    fsq_t* queue;
    void* data;
} fsq_param_t;

void* Consumers_5(void* param)
{
    int returned_data = 0;

    /*critical section start*/

    FSQDequeue(((fsq_param_t*)param)->queue, &returned_data, sizeof(int));
    printf("value after consumer: %d\n", returned_data);

    /*critical section end*/

    return NULL;
}

void* Producers_5(void* param)
{
    /*critical section start*/

    FSQEnqueue((((fsq_param_t*)param)->queue), ((fsq_param_t*)param)->data
                                                                , sizeof(int));
    printf("value after producer: %d\n", *(int*)((fsq_param_t*)param)->data);

    /*critical section end*/

    return NULL;
}

int EX5()
{
    size_t i = 0;

    fsq_t* queue = FSQCreate(CBUFFER_CAPACITY);
    int data[NUM_OF_THREADS_2] = {0};

    pthread_t thread_produce[NUM_OF_THREADS_2] = {0};
    pthread_t thread_consume[NUM_OF_THREADS_2] = {0};
    fsq_param_t parameters[NUM_OF_THREADS_2] = {0};

    for (i = 0; i < NUM_OF_THREADS_2; ++i)
    {
        data[i] = i + 1;
        parameters[i].queue = queue;
        parameters[i].data = &data[i];
    }

    for (i = 0; i < NUM_OF_THREADS_2; ++i)
    {
        pthread_create(&thread_produce[i], NULL, &Producers_5, &parameters[i]);
        pthread_create(&thread_consume[1], NULL, &Consumers_5, &parameters[i]);
    }

    pthread_exit(NULL);

    FSQDestroy(queue);

    return 0;
}

/*---EX6----------------------------------------------------------------------*/
pthread_mutex_t mutex_ex6 = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_ex6;
pthread_cond_t cond_flag;

void* Consumers_6(void* param)
{   
    size_t i = 0;
    
    for (i = 0; i < NUM_OF_ROUNDS; ++i)
    {
        /*critical section start*/
        pthread_mutex_lock(&mutex_ex6);
        
        sem_post(&sem_ex6);
        pthread_cond_wait(&cond_flag, &mutex_ex6);

        printf(RED"node value after consumer: %d, thread num: "B_RED"%ld\n"
                                                , *(int*)param, pthread_self());

        /*critical section end*/
        pthread_mutex_unlock(&mutex_ex6);   
    }

    return NULL;
}

void* Producer_6(void* param)
{
    size_t i = 0;
    int sem_val = 0;
    
    for (i = 0; i < NUM_OF_ROUNDS; ++i)
    {
        while (sem_val != NUM_OF_THREADS_2)
        {
            sem_getvalue(&sem_ex6, &sem_val);
        }
        
        /*critical section start*/
        pthread_mutex_lock(&mutex_ex6);

        *(int*)param += 1;
        printf(GR"node value after producer: %d, thread num: "B_GR"%ld\n"
                                                , *(int*)param, pthread_self());
        sem_val = 0;
        sem_init(&sem_ex6, 0, 0);
        pthread_cond_broadcast(&cond_flag);        
        
        /*critical section end*/
        pthread_mutex_unlock(&mutex_ex6);   
    }
    
    return NULL;
}

int EX6()
{
    size_t i = 0;
    int data = 0;

    pthread_t thread_produce = {0};
    pthread_t thread_consume[NUM_OF_THREADS_2] = {0};

    sem_init(&sem_ex6, 0, 0);
    pthread_cond_init(&cond_flag, NULL);

    pthread_create(&thread_produce, NULL, &Producer_6, &data); 

    for (i = 0; i < NUM_OF_THREADS_2; ++i)
    {
        pthread_create(&thread_consume[i], NULL, &Consumers_6, &data);
    } 

    pthread_join(thread_produce, NULL);

    for (i = 0; i < NUM_OF_THREADS_2; ++i)
    {
        pthread_join(thread_consume[i], NULL);
    }

    return 0;
}