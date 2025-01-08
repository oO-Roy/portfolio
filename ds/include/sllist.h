#ifndef __SLLIST_H__
#define __SLLIST_H__

#include <stddef.h>

typedef struct node *iterator_t;
typedef struct sllist sllist_t;

    /*
    @ description: creates a linked list data structure.
    @ params: this function doesn't take any parameters.
    @ return values: returns a pointer to the linked list.
    @ errors: returns NULL if memory allocation failed. 
    @ complexity: O(1)
    */
sllist_t *SLLCreate(void);
/*----------------------------------------------------------------------------*/
    /*
    @ description: SLLDestroy function frees memory that was 	allocated 
                   for the given list, and all subsequent inner allocation. 		
    @ params: sllist list pointer 
    @ return values: No return value.
    @ errors: No errors. 
    @ complexity: O(n)
    */
void SLLDestroy(sllist_t *list);
/*----------------------------------------------------------------------------*/
    /*
    @ description: Get the value at the data pointed to by iter. 
    @ params: iterator to get data from.
    @ return values: a pointer to the data stored at iter.
    @ errors: argument iter can't be the one returned from End function.
    @ complexity: O(1).
    */
void *SLLGetData(const iterator_t iter);
/*----------------------------------------------------------------------------*/  
    /*
    @ description: create new element in the linked list 
    	before the given iterator. 
    @ params: iter - the element to insert before, data - 
    	data of the new element.
    @ return values: iterator to the inserted element.
    @ errors: returns the SLListEnd iterator if memory allocation
              failed.
    @ complexity: O(1)
    */
iterator_t SLLInsertBefore(iterator_t iter, void *data);
/*----------------------------------------------------------------------------*/
    /*
    @ description: Remove element from list.
    @ params: iter - element to be removed.
    @ return values: No return value. 
    @ errors: if iter is end element (return value of SLListEnd), the behavior
              is undefined. 
    @ complexity: O(1)
    */
void SLLRemove(iterator_t iter);
/*----------------------------------------------------------------------------*/
    /*
    @ description: find element that matches the data parameter.
    @ params: 'IsMatch' is a match function tailored for user's data type,
              'IsMatch' params: pointer to data in element denoted by 
              	each iterator.
              in range and pointer to data sent as argument ('param').
    @ return values: if found - iterator holding matching data.
             else - to iterator.
    @ errors: no errors.
    @ complexity: O(n) (n is number of iterators between 'from' and 'to')
    */
iterator_t SLLFind(const iterator_t from, const iterator_t to,
    int (is_match) (const void *data, const void *param), void *param);
/*----------------------------------------------------------------------------*/
    /*
    @ description: compares between two iterators.
    @ params: one and other - two iterators to be compared. 
    @ return values: returns 1 if they are the same, 0 otherwise.
    @ errors: no errors.
    @ complexity: O(1)
    */
int SLLIsSameIter(const iterator_t one, const iterator_t other);
/*----------------------------------------------------------------------------*/
    /*
    @ description: return the iterator of the first element in the list.
    @ params: list - linked list to get the beginning of.
    @ return values: iterator_t
    @ errors: returns the first valid iterator in the linked list.
    @ complexity:O(1)
    Note-  When the list is empty, SLListBegin returns SLListEnd
    */
iterator_t SLLBegin(const sllist_t *list);
/*----------------------------------------------------------------------------*/
    /*
    @ description: The SSLNext() function return the adrress of the next 
                   iterator in the list. 
    @ params: iter - the iterator to get the next from.
    @ return values: returns the next iterator.
    @ errors: if list is empty - returns the End Iterator.
              if next is called on the end element - the behavior is undefined.
    @ complexity: O(1)
    */
iterator_t SLLNext(const iterator_t iter);
/*----------------------------------------------------------------------------*/
    /*
    @ description: returns the iterator of the last element in the linked 
                   list (End Iterator).
    @ params: const sllist *list 
    @ return values: iterator_t
    @ errors: if list is empty - returns the End Iterator.
    @ complexity: O(1)
    */
iterator_t SLLEnd(const sllist_t *list);
/*----------------------------------------------------------------------------*/
    /*
    @ description: Count and return the number of elements in the list
    @ params: list - the list to count the number of elements of.
    @ return values: element count.
    @ errors: no errors
    @ complexity: o(n)
    */
size_t SLLCount(const sllist_t *list);
/*----------------------------------------------------------------------------*/
    /*
    @ description: Applies action function on each element value.
    @ params: from and to define the range in the linked list to work on.
              action_func is a function pointer that will be applied on the
              the elements within the range (not including to).
              
    @ return values: none
    @ errors: no valid value - return;
    @ complexity: O(n);
    */
void SLLForEach(const iterator_t from, const iterator_t to
                , void (action_func) (void *data, void *param), void *param);
/*----------------------------------------------------------------------------*/
    /*
    @ description: Set the data in the iterator to the input data
    @ params: Iterator to edit. The iterator to set can't be null-terminated
    @ return value: This function doesn't return anything 
    @ errors: if the input iterator is null terminated, the function will do
        nothing
    @ complexity: O(1)
    */
void SLLSetData(const iterator_t iter, void* data);
/*----------------------------------------------------------------------------*/
    /*
    @ description: Check if given list is empty.
    @ params: list.
    @ return values: Returns 1 - if the list is empty, returns 0 - otherwise
    @ errors: 
    @ complexity: O(1)
    */
int SLLIsEmpty(const sllist_t *list);
/*----------------------------------------------------------------------------*/
/*
    @ description: Appends src sllist to dest sllist, 
    	after function call src sllist is empty.
    @ params: dest sllist, src sllist.
    @ return values: pointer to modified dest (sllist_t *).
    @ errors: If dest sllist param is equal to src sllist, 
    	behavior is undefined.
    @ complexity: O(1).
*/
sllist_t *SLLAppend(sllist_t *dest, sllist_t *src);

#endif /* __SLLIST_H__ */


