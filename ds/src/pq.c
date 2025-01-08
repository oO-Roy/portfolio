/* 
	Developer: Roy Steiner.
	File: Priority queue.
	Reviewer: Shimon Schwartz.
	Date: 09.07.2024
	Status: Approved.
*/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc & free */

#include "pq.h" /* typedef pq_t, pq_is_match, pq_comp_func 
											& forward declaration */
#include "sort_list.h" /* typedef sortl, sortl_iter_t & functions use */

#define EXTRACT_SORT_LIST(x) (x->list)

struct pqueue
{
	sortl_t *list;
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
	
	EXTRACT_SORT_LIST(pq) = SortLCreate(comp);
	if (NULL == EXTRACT_SORT_LIST(pq))
	{
		free(pq);
		return NULL;
	}
	
	return pq;
}

void PQDestroy(pq_t *pq)
{
	assert(NULL != pq);
	
	SortLDestroy(EXTRACT_SORT_LIST(pq));
	EXTRACT_SORT_LIST(pq) = NULL;
	
	free(pq);
	pq = NULL;
}

int PQEnqueue(pq_t *pq, const void *data)
{
	assert(NULL != pq);

	return SortLIsSameIter(SortLEnd(EXTRACT_SORT_LIST(pq))
					, SortLInsert(EXTRACT_SORT_LIST(pq), data));
}

void *PQDequeue(pq_t *pq)
{
	assert(NULL != pq);
	
	return SortLPopBack(EXTRACT_SORT_LIST(pq));
}

size_t PQSize(const pq_t *pq)
{
	assert(NULL != pq);
	
	return SortLSize(EXTRACT_SORT_LIST(pq));
}

int PQIsEmpty(const pq_t *pq)
{
	assert(NULL != pq);
	
	return SortLIsEmpty(EXTRACT_SORT_LIST(pq));
}

void *PQPeek(const pq_t *pq)
{
	assert(NULL != pq);
	
	return SortLGetVal(SortLPrev(SortLEnd(EXTRACT_SORT_LIST(pq))));
}

void PQClear(pq_t *pq)
{
	assert(NULL != pq);
	
	while (!PQIsEmpty(pq))
	{
		PQDequeue(pq); /* no use for return value */
	}
}

void *PQErase(pq_t *pq, pq_is_match_func is_match, void *data)
{
	sortl_iter_t iter = {0};
	void * return_data = NULL;
	
	assert(NULL != pq);
	assert(NULL != is_match);
	
	iter = SortLFindIf(SortLBegin(EXTRACT_SORT_LIST(pq))
									, SortLEnd(EXTRACT_SORT_LIST(pq))
									, is_match
									, data);
	if (SortLIsSameIter(iter, SortLEnd(EXTRACT_SORT_LIST(pq))))
	{
		return NULL;
	}
	
	return_data = SortLGetVal(iter);							
	SortLRemove(iter);	/* no use for return value */
	
	return return_data; 
}
