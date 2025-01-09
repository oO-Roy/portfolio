/* 
	File: Sorted list.
	Status: APPROVED.
*/

#include <stdlib.h>	/* malloc and free */
#include <assert.h> /* assert */

#include "sort_list.h" /* typedef, size_t */

struct sortl
{
	dllist_t *sort_list;
	sortl_comp_func comp;	
};

#ifndef NDEBUG
static sortl_t *DebugGetList(sortl_iter_t iter)
{
	return iter.list;
}
#endif

static dllist_t *SortListToDlList(const sortl_t *sortl)
{
	assert(NULL != sortl);
	
	return sortl->sort_list;
}

static dll_iter_t SortIterToDllIter(sortl_iter_t sort_iter)
{
	return sort_iter.iter;
}

static sortl_iter_t DllIterToSortIter(dll_iter_t dll_iter
		
		#ifndef NDEBUG
		, const sortl_t *list
		#endif 
															)
{
	sortl_iter_t sort_iter;
	
	assert(NULL != list);
	
	sort_iter.iter = dll_iter;
	
	#ifndef NDEBUG
		sort_iter.list = (sortl_t *)list;
	#endif
	
	return sort_iter;
}

static sortl_iter_t FindElement(const sortl_t *list, const sortl_iter_t from
											, const sortl_iter_t to
													, const void *data)
{
	sortl_iter_t runner_iter = from;
	
	assert(NULL != list);
	
	while (!SortLIsSameIter(runner_iter, to) 
						&& (0 > list->comp(SortLGetVal(runner_iter), data)))
	{
		runner_iter = SortLNext(runner_iter);
	}
	
	return runner_iter;
}

sortl_t *SortLCreate(sortl_comp_func comp)
{
	sortl_t * list = NULL;
	
	assert(NULL != comp);
	
	list = (sortl_t *) malloc(sizeof(sortl_t));
	if (NULL == list)
	{
		return NULL;
	}
	
	list->sort_list = DLlistCreate();
	if (NULL == list->sort_list)
	{
		free(list);
		return NULL;
	}

	list->comp = comp;
	
	return list;
}

void SortLDestroy(sortl_t *list)
{
	assert(NULL != list);
	
	DLlistDestroy(SortListToDlList(list));
	list->sort_list = NULL;
	free(list);
}

sortl_iter_t SortLInsert(sortl_t *list, const void *data)
{
	sortl_iter_t runner_iter = {0};
	
	assert(NULL != list);
	
	runner_iter = FindElement(list, SortLBegin(list), SortLEnd(list), data);
	
	return DllIterToSortIter(DLlistInsertBefore(SortIterToDllIter
			(runner_iter), data)
								#ifndef NDEBUG
								 , list
								#endif
								 	);
}

int SortLForEach(const sortl_iter_t from, const sortl_iter_t to
							, sortl_action_func action, void *param)
{
	assert(NULL != action);
	assert(DebugGetList(from) == DebugGetList(to));
	
	return DLlistForEach(SortIterToDllIter(from)
									, SortIterToDllIter(to), action, param);
}

sortl_iter_t SortLFind(const sortl_t *list, const sortl_iter_t from, 
                          const sortl_iter_t to, 
                          const void *data)
{	
	sortl_iter_t runner_iter = {0};
	
	assert(NULL != list);
	assert(DebugGetList(from) == DebugGetList(to));
	
	runner_iter = FindElement(list, from, to, data);
	
	if (0 == list->comp(SortLGetVal(runner_iter), data))
	{
		return runner_iter;
	}

	return to;
}

sortl_iter_t SortLFindIf(const sortl_iter_t from, const sortl_iter_t to
								, sortl_is_match_func is_match, void *param)
{
	assert(NULL != is_match);
	assert(DebugGetList(from) == DebugGetList(to));
	
	return DllIterToSortIter(DLlistFind(SortIterToDllIter(from)
								, SortIterToDllIter(to)
								,is_match, param) 
								#ifndef NDEBUG
								 , DebugGetList(from)
								#endif
								 );
}

sortl_t *SortLMerge(sortl_t *dest, sortl_t *src)
{
    sortl_iter_t d_current = {0};
    sortl_iter_t s_current = {0};
    sortl_iter_t s_next = {0};

    assert(NULL != dest);
    assert(NULL != src);
    assert(dest != src);

    d_current = SortLBegin(dest);
    s_current = SortLBegin(src);

	while (!SortLIsSameIter(s_current, SortLEnd(src)))
	{
		
		while ((!SortLIsSameIter(d_current, SortLEnd(dest)))
			&& (0 < dest->comp(SortLGetVal(s_current), SortLGetVal(d_current))))
		{
			d_current = SortLNext(d_current);
		}
		
		s_next = SortLNext(s_current); 	
		DLlistSplice(SortIterToDllIter(d_current), SortIterToDllIter(s_current)
   												, SortIterToDllIter(s_next));
		s_current = s_next;
	}
	
    return dest;
}

size_t SortLSize(const sortl_t *list)
{
	assert(NULL != list);
	
	return DLlistSize(SortListToDlList(list));
}

int SortLIsEmpty(const sortl_t *list)
{
	assert(NULL != list);
	
	return DLlistIsEmpty(SortListToDlList(list));
}

int SortLIsSameIter(const sortl_iter_t one, 
                       const sortl_iter_t other)
{
	return DLlistIsSameIter(SortIterToDllIter(one), SortIterToDllIter(other));
}

sortl_iter_t SortLBegin(const sortl_t *list)
{
	assert(NULL != list);
		
	return DllIterToSortIter(DLlistBegin(SortListToDlList(list))
												#ifndef NDEBUG
								 					, list
												#endif
								 					);
}

sortl_iter_t SortLEnd(const sortl_t *list)
{
	assert(NULL != list);
		
	return DllIterToSortIter(DLlistEnd(SortListToDlList(list))
												#ifndef NDEBUG
								 					, list
												#endif
								 					);
}

sortl_iter_t SortLNext(const sortl_iter_t iter)
{	
	return DllIterToSortIter(DLlistNext(SortIterToDllIter(iter))
												#ifndef NDEBUG
								 					, DebugGetList(iter)
												#endif
								 					);
}

sortl_iter_t SortLPrev(const sortl_iter_t iter)
{
	return DllIterToSortIter(DLlistPrev(SortIterToDllIter(iter))
												#ifndef NDEBUG
								 					, DebugGetList(iter)
												#endif
								 					);
}

void *SortLGetVal(const sortl_iter_t iter)
{	
	return DLlistGetVal(SortIterToDllIter(iter));
}

sortl_iter_t SortLRemove(sortl_iter_t iter)
{
	
	return DllIterToSortIter(DLlistRemove(SortIterToDllIter(iter))
													#ifndef NDEBUG
								 						, DebugGetList(iter)
													#endif
								 						);
}

void *SortLPopFront(sortl_t *list)
{
	assert(NULL != list);

	return DLlistPopFront(SortListToDlList(list));	
}

void *SortLPopBack(sortl_t *list)
{
	assert(NULL != list);

	return DLlistPopBack(SortListToDlList(list));
}
