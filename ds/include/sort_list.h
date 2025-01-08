#ifndef __SORT_LIST_H__
#define __SORT_LIST_H__

#include <stddef.h>	/* typedef size_t */

#include "dllist.h" /* typedef dll_iter_t */

typedef struct sortl_iter sortl_iter_t;
typedef struct sortl sortl_t;

typedef int (*sortl_comp_func) (const void *, const void *);
typedef int (*sortl_action_func) (void *, void *);
typedef int (*sortl_is_match_func) (const void *data, void *param);

struct sortl_iter
{
	dll_iter_t iter;
	#ifndef NDEBUG
		sortl_t *list;
	#endif
};

/*
    @ Description: Creates an empty sort_list.
    @ Params: Pointer to function 'comp_func'.
    @ Return Values: Returns a sortl_t pointer to the new empty sort_list.
    @ Errors: Returns NULL if memory allocation failed.
    @ Time complexity: O(1).
    @ Space complexity: O(1).
*/
sortl_t *SortLCreate(sortl_comp_func comp);

/*
    @ Description: Inserts the data into the correct place 
    													within the sort_list.
    @ Params: sortl_t pointer to list, constant void pointer to data.
    @ Return value: sortl_iter_t iterator.
    @ Errors: If malloc fails, function returns the end element. 
    @ Complexity: O(n). 
    @ Space complexity: O(1).
*/
sortl_iter_t SortLInsert(sortl_t *list, const void *data);   

/*
    @ Description: The function will find the required parameter in
    												 the range inside sort_list.
    @ Params: Constant sortl_t pointer to sort_list
    					, const sortl_iter_t iterator (from)
    						, constant sortl_iter_t iterator (to)
    							, constant void pointer to parameter.
    @ Return value: Found sortl_iter_t iterator with the valid parameter.
    								If value not found - return 'to' iterator.
    @ Errors: None.
    @ Complexity: O(n). 
    @ Space complexity: O(1).
*/
sortl_iter_t SortLFind(const sortl_t *list, const sortl_iter_t from, 
                          const sortl_iter_t to, 
                          const void *data);

/*
    @ Description: The Function finds an element by an a condition that is set
    											by a compare (user) function.
    @ Params: Constant sortl_iter_t iterator 'from'
    				, constant sortl_iter_t iterator 'to'
    				, sortl_is_match_func pointer to is_match function
    				, void pointer to parameter.
    @ Return value: sortl_iter_t iterator.
    @ Errors: None.
    @ Time complexity: O(n). 
    @ Space complexity: O(1).
*/
sortl_iter_t SortLFindIf(const sortl_iter_t from, const sortl_iter_t to
								, sortl_is_match_func is_match, void *param);
                    
/*
    @ Description: The function will merge sort_list into another.
    @ Params: sortl_t pointer to 'dest' sort_list, sortl_t pointer to 'src' 
    																sort_list.
    @ Return value: sortl_t pointer to updated 'dest' sort_list. 
    @ Errors: None.
    @ Time complexity: O(n).
    @ Space complexity: O(1).
*/ 
sortl_t *SortLMerge(sortl_t *dest, sortl_t *src);     

/*
    @ Description: Applies action function on each element value.
    @ Params: Constant sortl_iter_t iterator  (from)
    				, constant sortl_iter_t iterator  (to)
    								, function pointer (action)
    										, void pointer to parameter.
    @ Return value: Return 0 if the action function success, otherwise 1.
    @ Errors: None.
    @ Complexity: O(n). 
    @ Space complexity: O(1).
*/
int SortLForEach(const sortl_iter_t from, const sortl_iter_t to
							, sortl_action_func action, void *param);
							
/*
    @ Description: Counts the number of elements in the sort_list.
    @ Params: Constant sortl_t pointer to sort_list.
    @ Return value: The number of elements in the sort_list 
    @ Errors: None.
    @ Complexity: O(1).
    @ Space complexity: O(1).
*/
size_t SortLSize(const sortl_t *list);

/*
    @ Description: Checks if the sort_list is empty.
    @ Params: Constant sortl_t pointer to sort_list. 
    @ Return value: Returns 1 if the sort_list is empty, otherwise 0.
    @ Errors: None.
    @ Complexity: O(1).
    @ Space complexity: O(1).
*/
int SortLIsEmpty(const sortl_t *list);

/*
    @ Description: Compares between two sort_list iterators.
    @ Params: Two constant sortl_iter_t iterators (one & other).
    @ Return value: Returns 1 if they are the same, otherwise 0.
    @ Errors: None.
    @ Time complexity: O(1).
    @ Space complexity: O(1).
*/
int SortLIsSameIter(const sortl_iter_t one, 
                       const sortl_iter_t other);

/*
    @ Description: Returns the next iterator.
    @ Params: Constant sortl_iter_t to iter.
    @ Return value: The next iterator.
    @ Errors: None.DllIterToSortIter
    @ Time complexity: O(1).
    @ Space complexity: O(1).
*/
sortl_iter_t SortLNext(const sortl_iter_t iter);

/*
    @ Description: Returns the previous iterator.
    @ Params: Constant sortl_iter_t to iter.
    @ Return value: The previous iterator.
    @ Errors: None.
    @ Time complexity: O(1).
    @ Space complexity: O(1).
*/
sortl_iter_t SortLPrev(const sortl_iter_t iter);

/*
    @ Description: Returns an sortl_iter_t iterator to the first valid element.
    @ Params: Constant sort_l pointer to sort_list.
    @ Return value: sortl_iter_t iterator to the first valid element.
    @ Errors: None.
    @ Time complexity: O(1). 
    @ Space complexity: O(1).
*/
sortl_iter_t SortLBegin(const sortl_t *list);

/*
    @ Description: Returns an sortl_iter_t iterator to the last element.
    @ Params: Constant sort_l pointer to sort_list.
    @ Return value: sortl_iter_t iterator to the last element.
    @ Errors: None.
    @ Time complexity: O(1). 
    @ Space complexity:
*/
sortl_iter_t SortLEnd(const sortl_t *list);

/*
    @ Description: Gets the data value of sortl_iter_t iterator .
    @ Params: Constant sortl_iter_t iterator. 
    @ Return value: Void pointer.
    @ Errors: If iterator is end of sort-list - undefined behavior.
    @ Complexity: O(1). 
    @ Space complexity: O(1).
*/
void *SortLGetVal(const sortl_iter_t iter);

/*
    @ Description: Frees memory allocation of sort_list.
    @ Params: sortl_t pointer to sort_list. 
    @ Return value: None.
    @ Errors: None.
    @ Time complexity: O(n).
    @ Space complexity O(1).
*/
void SortLDestroy(sortl_t *list);

/*
    @ Description: Frees memory allocation to a given iterator.
    @ Params: sortl_iter_t iterator.
    @ Return value: sortl_iter_t to next iterator.
    @ Errors: If iter is end of sort_list - undefined behavior.
    @ Complexity: O(1).
    @ Space complexity: O(1).
*/
sortl_iter_t SortLRemove(sortl_iter_t iter);

/*
    @ Description: Pops the first valid element in sort_list.
    @ Params: sortl_t pointer to sort_list.
    @ Return value: Void pointer to the data of removed element. 
    @ Errors: None.
    @ Complexity: O(1). 
    @ Space complexity: O(1).
*/
void *SortLPopFront(sortl_t *list);

/*
    @ Description: Pops the last valid element in sort_list.
    @ Params: sortl_t pointer to sort_list.
    @ Return value: Void pointer to the data of removed element. 
    @ Errors: None.
    @ Complexity: O(1). 
    @ Space complexity: O(1).
*/
void *SortLPopBack(sortl_t *list);

#endif /* __SORT_LIST_H__ */
