#ifndef __DLLIST_H__
#define __DLLIST_H__

#include <stddef.h> /* size_t */

typedef struct dllist dllist_t;
typedef struct dll_node *dll_iter_t;

typedef int (*action_func)(void *data, void *param);
typedef int (*is_match_func) (const void *data, void *param);

/*
    @ Description: Creates an empty dllist.
    @ Params: No parametes.
    @ Return Values: Returns a pointer to the new empty dllist.
    @ Errors: Returns NULL if memory allocation failed.
    @ Complexity: O(1).
*/
dllist_t *DLlistCreate(void);

/*
    @ Description: Frees memory allocation of dllist.
    @ Params: dllist_t pointer to dllist. 
    @ Return value: None.
    @ Errors: None.
    @ Complexity: O(1). 
*/
void DLlistDestroy(dllist_t *dllist);

/*
    @ Description: Inserts the data pointed to by iterator into the dllist.
    @ Params: Pointer to iterator, constant void pointer to data.
    @ Return value: Pointer to iterator.
    @ Errors: 1) If malloc fails function returns the end element.
    		  2) If iterator is end of dllist - undefined behavior. 
    @ Complexity: O(1).
*/
dll_iter_t DLlistInsertBefore(dll_iter_t iter, const void *data);

/*
    @ Description: The function finds all of iterators (within range) that 
   									stores 'parameter' and creates duplicated 
   											  iterators to user's extended list. 
    @ Params: Constant iterator pointer (from), constant iterator pointer (to)
    											  , function pointer (is_match)
    											    , void pointer to parameter
    											    , pointer to extended list.
    @ Return value: If data found returns 1, otherwise 0.
    @ Errors: None.
    @ Complexity: O(n).
*/
int DLlistFindAll(const dll_iter_t from, const dll_iter_t to, is_match_func func
											, void *param, dllist_t *output);

/*
    @ Description: The function will find the required parameter in the range
    															inside dllist.
    @ Params: Constant iterator pointer (from), constant iterator pointer (to)
    											  , function pointer (is_match)
    											    , void pointer to parameter.
    @ Return value: Found iterator pointer with the valid parameter.
    								If value not found - return 'to' iterator.
    @ Errors: None.
    @ Complexity: O(n).
*/
dll_iter_t DLlistFind(const dll_iter_t from, const dll_iter_t to, is_match_func 
															func, void *param);

/*
    @ Description: Applies action function on each element value.
    @ Params: Constant iterator pointer (from), constant iterator pointer (to)
    											  , function pointer (action)
    											    , void pointer to parameter.
    @ Return value: Return 0 if the action function success, otherwise 1.
    @ Errors: None.
    @ Complexity: O(n).
*/
int DLlistForEach(const dll_iter_t from, const dll_iter_t to, action_func func
																, void *param);
/*
    @ Description: Checks if the dllist is empty.
    @ Params: Constant pointer to dllist. 
    @ Return value: Returns 1 if the dllist is empty, otherwise 0.
    @ Errors: None.
    @ Complexity: O(1). 
*/
int DLlistIsEmpty(const dllist_t *dllist);

/*
    @ Description: Compares between two dllist iterators.
    @ Params: Two constant dllist iterators (one & other).
    @ Return value: Returns 1 if they are the same, otherwise 0.
    @ Errors: None.
    @ Complexity: O(1).
*/
int DLlistIsSameIter(const dll_iter_t one, const dll_iter_t other);

/*
    @ Description: Function adds iterators 'from' until 'to' (not included) 
    			from one dllist to another, before the iterator 'destination'. 
    @ Params: Iterator pointer 'destination',
    								Iterator pointer 'from',
    											Iterator pointer 'to'. 
    @ Return value: None.
    @ Errors: None.
    @ Complexity: O(1).
*/
void DLlistSplice(dll_iter_t dest, dll_iter_t from, dll_iter_t to);

/*
    @ Description: Frees memory allocation to a given iterator.
    @ Params: dll_iter_t iterator.
    @ Return value: Pointer to next iterator.
    @ Errors: If iter is end of dllist - undefined behavior.
    @ Complexity: O(1).
*/
dll_iter_t DLlistRemove(dll_iter_t iter);

/*
    @ Description: Sets data value to iterator pointer;
    @ Params: Iterator pointer, constant pointer to data;
    @ Return value: None.
    @ Errors: If iterator is end of dllist - undefined behavior.
    @ Complexity: O(1).
*/
void DLlistSetVal(dll_iter_t iter, const void *data);

/*
    @ Description: Gets the data value of iterator pointer.
    @ Params: Constant pointer to iterator. 
    @ Return value: Void pointer.
    @ Errors: If iterator is end of dllist - undefined behavior.
    @ Complexity: O(1). 
*/
void *DLlistGetVal(const dll_iter_t iter);

/*
    @ Description: Returns the next iterator.
    @ Params: Constant pointer to iter.
    @ Return value: The next iterator.
    @ Errors: None.
    @ Complexity: O(1). 
*/
dll_iter_t DLlistNext(const dll_iter_t iter);

/*
    @ Description: Returns the previous iterator.
    @ Params: Constant pointer to iter.
    @ Return value: The previous iterator.
    @ Errors: None.
    @ Complexity: O(1). 
*/
dll_iter_t DLlistPrev(const dll_iter_t iter);

/*
    @ Description: Returns an dll_iter_t iterator pointer to 
    													the end element.
    @ Params: Pointer to dllist_t dllist.
    @ Return value: dll_iter_t iter pointer to the end element.
    @ Errors: None. 	
    @ Complexity: O(1).
*/
dll_iter_t DLlistEnd(const dllist_t *dllist);

/*
    @ Description: Returns an dll_iter_t iterator pointer to 
    													the first valid element.
    @ Params: Constant pointer to dllist.
    @ Return value: dll_iter_t iterator pointer to the first valid element.
    @ Errors: None.
    @ Complexity: O(1).
*/
dll_iter_t DLlistBegin(const dllist_t *dllist);

/*
    @ Description: Counts the number of elements in the dllist.
    @ Params: Constant dllist_t pointer to dllist.
    @ Return value: The number of elements in the dllist 
    @ Errors: None.
    @ Complexity: O(1).
*/
size_t DLlistSize(const dllist_t *dllist);

/*
    @ Description: Push new element at the end of the dllist.
    @ Params: Dllist pointer, constant pointer for data.
    @ Return value: Iterator to the pushed element.
    @ Errors: If malloc fails return end element.
    @ Complexity: O(1).
*/
dll_iter_t DLlistPushBack(dllist_t *dllist, const void *data);

/*
    @ Description: Push new element at the beginning of the dllist.
    @ Params: Dllist pointer, constant pointer for data.
    @ Return value: Iterator to the pushed element.
    @ Errors: If malloc fails return end element.
    @ Complexity: O(1).
*/
dll_iter_t DLlistPushFront(dllist_t *dllist, const void *data);

/*
    @ description: removes the last valid element in a DLl
    @ params: list (doubly linked list)
    @ return value: Void pointer to the data of the element we removed
    @ errors: no errors
    @ complexity: O(1) 
*/
void *DLlistPopBack(dllist_t *dllist);

/*
    @ Description: Pops the first valid element in dllist.
    @ Params: Dllist pointer.
    @ Return value: Void pointer to the data of removed element. 
    @ Errors: None.
    @ Complexity: O(1).
*/
void *DLlistPopFront(dllist_t *dllist);

#endif /* __DLLIST_H__ */
