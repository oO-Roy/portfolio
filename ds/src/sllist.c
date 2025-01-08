#include <assert.h>		/* For assert */
#include <stdlib.h>		/* For malloc & free*/  

#include "sllist.h"

#define UNUSED(x) (void)(x)

struct node
{
	void *data;
	struct node *next;
};

struct sllist
{
	struct node *head;
	struct node *tail;
};

sllist_t *SLLCreate(void)
{	
	sllist_t *list = (sllist_t *) malloc(sizeof(sllist_t));
	list->tail = (struct node *) malloc(sizeof(struct node));
	
	if (NULL == list->tail || NULL == list)
	{
		free(list); list = NULL;
		return NULL;
	}
	
	list->tail->data = &list->tail;
	list->tail->next = NULL;
	
	list->head = list->tail; 
	
	return list;
}

void SLLDestroy(sllist_t *list)
{
	iterator_t tmp = NULL;
	
	assert(NULL != list);
	
	while (!SLLIsSameIter(list->head, list->tail))
	{
		tmp = list->head->next;
		free(list->head);
		list->head = tmp;
	}
	
	free(list->tail); list->tail = NULL;								
	free(list); list = NULL;	 
}

void *SLLGetData(const iterator_t iter)
{
	assert(NULL != iter);
	
	return ((NULL == iter->next) ? (NULL) : (iter->data));
}

iterator_t SLLInsertBefore(iterator_t iter, void *data)
{
	iterator_t node = NULL;
	
	assert(NULL != iter);
	
	node = (struct node *) malloc(sizeof(struct node));
	if (NULL == node)
	{
		while (NULL != iter->next)
		{
			iter = SLLNext(iter);
		}
	
		return iter;
	}
	
	node->data = iter->data;
	node->next = iter->next;
	iter->data = data;
	iter->next = node;
	
	if (NULL == node->next)
	{
		*((struct node **)(node->data)) = node;
	}
	
	return iter;
}

void SLLRemove(iterator_t iter)
{
	iterator_t tmp = iter->next;
	
	assert(NULL != iter);
			
	iter->data = tmp->data;
	iter->next = tmp->next;		

	tmp->data = NULL;
	tmp->next = NULL;
	
	free(tmp); tmp = NULL;
	
	if (NULL == iter->next)
	{
		*((struct node **) (iter->data)) = iter;
	}
}

iterator_t SLLFind(const iterator_t from, const iterator_t to,
    int (is_match) (const void *data, const void *param), void *param)
{	
	iterator_t tmp = from;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(is_match);

	while ((!SLLIsSameIter(tmp, to)) && (!is_match(tmp->data, param)))
	{
		tmp = SLLNext(tmp);
	}
	
	return tmp;
}

int SLLIsSameIter(const iterator_t one, const iterator_t other)
{
	assert(NULL != one);
	assert(NULL != other);
	
	return (one == other);
}

iterator_t SLLBegin(const sllist_t *list)
{
	assert(NULL != list);
	
	return list->head;
}

iterator_t SLLNext(const iterator_t iter)
{
	assert(NULL != iter);
	
	return iter->next;
}

iterator_t SLLEnd(const sllist_t *list)
{
	assert(NULL != list);
	
	return list->tail;
}

static void CountingElements(void *data, void *param)
{
	UNUSED(data);
	
	++(*(size_t *)param);
}

size_t SLLCount(const sllist_t *list)
{
	size_t counter = 0;
	
	assert(NULL != list);
	
	SLLForEach(list->head, list->tail, CountingElements, &counter);
	
	return counter;
}

void SLLForEach(const iterator_t from, const iterator_t to, 
		void (action_func) (void *data, void *param), void *param)
{
		iterator_t tmp = from;
		
		assert(NULL != from);
		assert(NULL != to);
		assert(action_func);
		
		while (!SLLIsSameIter(tmp, to))
		{
			action_func(tmp->data, param);
			tmp = SLLNext(tmp);
		}
}

void SLLSetData(const iterator_t iter, void *data)
{
	assert(NULL != iter);
	
	if (NULL != iter->next)
	{
		iter->data = data;
	}
}

int SLLIsEmpty(const sllist_t *list)
{
	assert(NULL != list);
	
	return (list->head == list->tail);
}

sllist_t *SLLAppend(sllist_t *dest, sllist_t *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	assert(src != dest);
	
	/* Copy src_first to dest_dummy */
	dest->tail->next = src->head->next;
	dest->tail->data = src->head->data;
	
	/* Update "dest tail" */
	dest->tail = src->tail;
	dest->tail->data = &dest->tail;
	
	/* Make src an empty list */
	src->tail = src->head;
	src->tail->data = &src->tail;
	src->tail->next = NULL;
	
	return dest;
}
