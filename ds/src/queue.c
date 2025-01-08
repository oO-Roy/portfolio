#include <assert.h> /* for assert use */
#include <stdlib.h> /* for malloc use */

#include "queue.h"	/* forward decleration */
#include "sllist.h" /* for the reuse of SLList API */

struct queue
{
	sllist_t *queue_list;
};

queue_t *QueueCreate(void)
{
	queue_t *queue = (queue_t *) malloc(sizeof(queue_t));
	
	if (NULL == queue)
	{
		return NULL;
	}
	
	queue->queue_list = SLLCreate();
	
	return ((NULL == queue->queue_list) ? (NULL) : (queue));
}

void QueueDestroy(queue_t *queue)
{
	assert(NULL != queue);
	
	SLLDestroy(queue->queue_list);
	
	free(queue); queue = NULL;
}

int QueueEnqueue(queue_t *queue, const void *data)
{
	assert(NULL != queue);
	
	return (NULL == SLLNext((SLLInsertBefore(SLLEnd(queue->queue_list), (void *) data))));
}

void QueueDequeue(queue_t *queue)
{
	assert(NULL != queue);
	
	SLLRemove(SLLBegin(queue->queue_list));
}

size_t QueueSize(const queue_t *queue)
{
	assert(NULL != queue);
	
	return (SLLCount(queue->queue_list));
}

int QueueIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);
	
	return (SLLIsEmpty(queue->queue_list));
}

void *QueuePeek(const queue_t *queue)
{
	assert(NULL != queue);
	
	return (SLLGetData(SLLBegin(queue->queue_list)));
}

queue_t *QueueAppend(queue_t *dest, queue_t *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	assert(dest != src);
	
	return ((queue_t *) SLLAppend(dest->queue_list, src->queue_list));
}
