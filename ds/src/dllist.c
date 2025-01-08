#include <stdio.h>
#include <assert.h>
#include <stdlib.h>	/* malloc, free */

#include "dllist.h"	/* typedef, size_t */

#define UNUSED(x) (void)(x)
#define ITER_TO_NODE(iter) ((struct dll_node *) iter)
#define NODE_TO_ITER(node) ((dll_iter_t) node) 

struct dll_node
{
	struct dll_node *next;
	struct dll_node *prev;
	void *data;
};

struct dllist
{
	struct dll_node head;
	struct dll_node tail;
};

dllist_t *DLlistCreate(void)
{
	dllist_t *dllist = NULL;
	
	dllist = (dllist_t *) malloc(sizeof(struct dllist));
	if (NULL == dllist)
	{
		return NULL;
	}

	dllist->head.next = &dllist->tail;
	dllist->tail.prev = &dllist->head;
	
	dllist->head.prev = NULL;
	dllist->tail.next = NULL;
	
	return dllist;
}

dll_iter_t DLlistNext(const dll_iter_t iter)
{
	assert(NULL != iter);
	
	return ITER_TO_NODE(iter)->next;
}

dll_iter_t DLlistPrev(const dll_iter_t iter)
{
	assert(NULL != iter);
	
	return ITER_TO_NODE(iter)->prev;
}

dll_iter_t DLlistRemove(dll_iter_t iter)
{
	dll_iter_t next_iter = DLlistNext(iter);
	dll_iter_t prev_iter = DLlistPrev(iter);
	
	assert(NULL != iter);

	ITER_TO_NODE(next_iter)->prev = prev_iter;
	ITER_TO_NODE(prev_iter)->next = next_iter;
	 
	ITER_TO_NODE(iter)->next = NULL;
	ITER_TO_NODE(iter)->prev = NULL;
	ITER_TO_NODE(iter)->data = NULL;
	
	free(iter);
	
	return next_iter;
}

dll_iter_t DLlistEnd(const dllist_t *dllist)
{
	assert(NULL != dllist);
	
	return NODE_TO_ITER((&dllist->tail));
}

dll_iter_t DLlistBegin(const dllist_t *dllist)
{
	assert(NULL != dllist);
	
	return DLlistNext(NODE_TO_ITER(&dllist->head));
}

int DLlistIsEmpty(const dllist_t *dllist)
{
	assert(NULL != dllist);

	return (DLlistNext(NODE_TO_ITER(&dllist->head)) 
						== NODE_TO_ITER(&dllist->tail));
}

int DLlistIsSameIter(const dll_iter_t one, const dll_iter_t other)
{
	assert(NULL != one);
	assert(NULL != other);
	
	return (one == other);
}

void DLlistSplice(dll_iter_t dest, dll_iter_t from, dll_iter_t to) 
{
	dll_iter_t dest_prev = DLlistPrev(dest);
	dll_iter_t from_prev = DLlistPrev(from);
	dll_iter_t to_prev = DLlistPrev(to);
	
	assert(NULL != dest);
	assert(NULL != from);
	assert(NULL != to);
	
	ITER_TO_NODE(dest_prev)->next = from;
	ITER_TO_NODE(from)->prev = dest_prev;
	
	ITER_TO_NODE(to_prev)->next = dest;
	ITER_TO_NODE(dest)->prev = to_prev;
	
	ITER_TO_NODE(from_prev)->next = to;
	ITER_TO_NODE(to)->prev = from_prev;
}

void DLlistSetVal(dll_iter_t iter, const void *data)
{
	assert(NULL != iter);

	ITER_TO_NODE(iter)->data = (void *) data;
}

void *DLlistGetVal(const dll_iter_t iter)
{
	assert(NULL != iter);
	
	return ((void *) ITER_TO_NODE(iter)->data);
}

static int CountingElements(void *data, void *param)
{
	UNUSED(data);
	
	++(*(size_t *)param);
	
	return 0;
}

size_t DLlistSize(const dllist_t *dllist)
{
	size_t counter = 0;
	
	assert(NULL != dllist);
	
	DLlistForEach(DLlistNext(NODE_TO_ITER(&dllist->head)), NODE_TO_ITER(&dllist->tail), CountingElements, &counter);
	
	return counter;
}

dll_iter_t DLlistInsertBefore(dll_iter_t iter, const void *data)
{
	dll_iter_t new_iter = NULL;
	dll_iter_t prev_iter = DLlistPrev(iter);	 
	
	assert(NULL != iter);
	
	new_iter = (dll_iter_t) malloc(sizeof(struct dll_node));
	if (NULL == new_iter)
	{
		while (NULL != DLlistNext(iter))
		{
			iter = DLlistNext(iter);
		}
	
		return iter;
	}
	
	ITER_TO_NODE(prev_iter)->next = new_iter;
	ITER_TO_NODE(iter)->prev = new_iter;
	
	ITER_TO_NODE(new_iter)->next = iter;
	ITER_TO_NODE(new_iter)->prev = prev_iter;
	ITER_TO_NODE(new_iter)->data = (void *) data;
	
	return new_iter;
}

int DLlistFindAll(const dll_iter_t from, const dll_iter_t to, is_match_func func
											, void *param, dllist_t *output)
{
	dll_iter_t from_iter = (dll_iter_t) from;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != func);
	assert(NULL != output);

	while (!DLlistIsSameIter(from_iter, to)) 
	{
		from_iter = DLlistFind(from_iter, to, func, param);
		
		if (DLlistIsSameIter(from_iter, to))
		{
			return 0;
		}
		
		DLlistInsertBefore(DLlistEnd(output)
							, ITER_TO_NODE(from_iter)->data);
		from_iter = DLlistNext(from_iter);
	}
	
	return 1;
}

dll_iter_t DLlistFind(const dll_iter_t from, const dll_iter_t to
											, is_match_func func, void *param)
{
	dll_iter_t from_iter = (dll_iter_t) from;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != func);
	
	while ((!DLlistIsSameIter(from_iter, to)) 
						&& (!func(ITER_TO_NODE(from_iter)->data, param)))
	{
		from_iter = DLlistNext(from_iter);
	}

	return from_iter;
}

int DLlistForEach(const dll_iter_t from, const dll_iter_t to, action_func func
																, void *param)
{
	int action_func_status = 0;
	dll_iter_t from_iter = (dll_iter_t) from;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != func);
	
	while ((!DLlistIsSameIter(from_iter, to)) && (1 != action_func_status))
	{
		action_func_status = func(ITER_TO_NODE(from_iter)->data, param);
		from_iter = DLlistNext(from_iter);
	}
	
	return action_func_status;
}

dll_iter_t DLlistPushBack(dllist_t *dllist, const void *data)
{
	dll_iter_t new_iter = NULL;
	
	assert(NULL != dllist);
	
	new_iter = DLlistInsertBefore(DLlistEnd(dllist), data);
	
	return new_iter;
}

dll_iter_t DLlistPushFront(dllist_t *dllist, const void *data)
{
	assert(NULL != dllist);
	
	return DLlistInsertBefore(DLlistBegin(dllist), data);
}

void *DLlistPopBack(dllist_t *dllist)
{
	void *data = NULL;
	
	assert(NULL != dllist);

	data = DLlistGetVal(DLlistPrev(DLlistEnd(dllist)));
	DLlistRemove(DLlistPrev(DLlistEnd(dllist)));	
	
	return data;
}

void *DLlistPopFront(dllist_t *dllist)
{
	void *data = NULL;
	assert(NULL != dllist);

	data = DLlistGetVal(DLlistBegin(dllist));
	DLlistRemove(DLlistBegin(dllist));

	return data;
}

void DLlistDestroy(dllist_t *dllist)
{
	dll_iter_t remove_iter = NULL;
	dll_iter_t tmp = NULL;
	
	assert(NULL != dllist);
	
	remove_iter = DLlistBegin(dllist);
	
	while (NULL != DLlistNext(remove_iter))
	{
		tmp = DLlistNext(remove_iter);
		DLlistRemove(remove_iter);
		remove_iter = tmp;
	}
	
	remove_iter = NULL;
	
	free(dllist); dllist = NULL;
}
