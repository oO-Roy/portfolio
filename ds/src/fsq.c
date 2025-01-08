#include <stdlib.h> /*malloc & free*/
#include <assert.h>    /*assert*/
#include <semaphore.h> /*semaphores*/
#include <pthread.h>   /*mutex*/

#include "fsq.h"    /*forward declaration*/
#include "cbuffer.h" /*container*/

struct fsq
{
    cbuff_t* buffer;
    sem_t sem_write;
    sem_t sem_read;
    pthread_mutex_t mutex_write;
    pthread_mutex_t mutex_read;
};

static cbuff_t* GetBuffer(fsq_t *queue)
{
    assert(queue);

    return queue->buffer;
}

fsq_t* FSQCreate(size_t capacity)
{
    fsq_t *queue = malloc(sizeof(fsq_t));
    if (NULL == queue)
    {
        return NULL;
    }

    queue->buffer = CBuffCreate(capacity);
    if (NULL == GetBuffer(queue))
    {
        free(queue);
        return NULL;
    }

    sem_init(&queue->sem_read, 0, 0);
    sem_init(&queue->sem_write, 0, capacity);

    pthread_mutex_init(&queue->mutex_write, NULL);
    pthread_mutex_init(&queue->mutex_read, NULL);

    return queue;
}

void FSQDestroy(fsq_t *queue)
{
    assert(queue);

    CBuffDestroy(GetBuffer(queue));
    
    sem_destroy(&queue->sem_write);
    sem_destroy(&queue->sem_read);
    
    pthread_mutex_destroy(&queue->mutex_write);
    pthread_mutex_destroy(&queue->mutex_read);
    
    free(queue);
}

int FSQEnqueue(fsq_t *queue, const void* data, size_t n)
{
    assert(queue);

    sem_wait(&queue->sem_write);
    /*start of critical section*/
    pthread_mutex_lock(&queue->mutex_write);

    CBuffWrite(GetBuffer(queue), (void*)data, n);
    
    sem_post(&queue->sem_read);
    /*end of critical section*/
    pthread_mutex_unlock(&queue->mutex_write);

    return 0;
}

void FSQDequeue(fsq_t *queue, void* data, size_t n)
{
    assert(queue);

    sem_wait(&queue->sem_read);
    /*start of critical section*/
    pthread_mutex_lock(&queue->mutex_read);

    CBuffRead(GetBuffer(queue), data, n);

    sem_post(&queue->sem_write);
    /*end of critical section*/
    pthread_mutex_unlock(&queue->mutex_read);
}
