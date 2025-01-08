/* 
	Developer: Roy Steiner.
	File: Priority queue by heap DS.
	Reviewer: 
	Date: 
	Status: 
*/
#include <stdlib.h> /*malloc & free*/
#include <assert.h> /*assert*/

#include "pq_by_heap.h" /*size_t*/
#include "heap.h" /*API*/

#define EXTRACT_LIST(x) (x->list)

struct pqueue
{
	heap_t* list;
};

pq_t *PQCreate(pq_comp_func comp)
{	
	pq_t *pq = NULL;
	
	assert(NULL != comp);
	
	pq = (pq_t *) malloc(sizeof(pq_t));
	if (NULL == pq)
	{
		return NULL;
	}
	
	EXTRACT_LIST(pq) = HeapCreate(comp);
	if (NULL == EXTRACT_LIST(pq))
	{
		free(pq);
		return NULL;
	}
	
	return pq;
}

void PQDestroy(pq_t *pq)
{
	assert(NULL != pq);
	
	HeapDestroy(EXTRACT_LIST(pq));
	
	free(pq);
}

int PQEnqueue(pq_t *pq, const void *data)
{
	assert(NULL != pq);

	return HeapPush(EXTRACT_LIST(pq), data);
}

void *PQDequeue(pq_t *pq)
{
	assert(NULL != pq);
	
	return HeapPop(EXTRACT_LIST(pq));
}

size_t PQSize(const pq_t *pq)
{
	assert(NULL != pq);
	
	return HeapSize(EXTRACT_LIST(pq));
}

int PQIsEmpty(const pq_t *pq)
{
	assert(NULL != pq);
	
	return HeapIsEmpty(EXTRACT_LIST(pq));
}

void *PQPeek(const pq_t *pq)
{
	assert(NULL != pq);
	
	return HeapPeek(EXTRACT_LIST(pq));
}

void PQClear(pq_t *pq)
{
	assert(NULL != pq);
	
	while (!PQIsEmpty(pq))
	{
		HeapPop(EXTRACT_LIST(pq)); /* no use for return value */
	}
}

void *PQErase(pq_t *pq, pq_is_match_func is_match, void *data)
{
	assert(NULL != pq);
	assert(NULL != is_match);

	return HeapRemove(EXTRACT_LIST(pq), is_match, data); 
}
