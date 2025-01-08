#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h> /*size_t*/

typedef int (*heap_comp_func) (const void* data, const void* param);
typedef struct heap heap_t; 

/* -----------------------------------------------------------------------------
    @ description:      This function return a pointer to new heap, where the comparison
                        between keys in the heap is done using the comparison function
                        provided as a parameter.    
    
    @ params:           comp_func: A comparison function of type `heap_comp_func` that
                        compares two keys in the tree. The function should take two

    @ errors:           return NULL when memory allocation fail.  
    
    @ time complexity:  O(1)
    
    @ space complexity: O(1)
-----------------------------------------------------------------------------*/
heap_t* HeapCreate(heap_comp_func comp_func);

/* -----------------------------------------------------------------------------
    @ description:      Releases the memory allocated for the heap.
    
    @ params:           A pointer to a heap created by HeapCreate.

    @ time complexity:  O(1).
    
    @ space complexity: O(1).
-----------------------------------------------------------------------------*/
void HeapDestroy(heap_t* heap);

/* -----------------------------------------------------------------------------
    @ description:      Inserts a data element into the heap data structure.    
    
    @ params:           'heap' - a heap data structure made by HeapCreate.
                        'data' - pointer to the data to be stored.
    
    @ return value:     returns 0 if push was successful and 1 otherwise.
    
    @ errors:           memory allocation failure. 
    
    @ time complexity:  O(log n)
    
    @ space complexity: avarage case: O(1)
                        worst case:   O(n) (for when realloc is needed in the vector)
-----------------------------------------------------------------------------*/
int HeapPush(heap_t* heap, const void* data);

/* -----------------------------------------------------------------------------
    @ description: Pop the top (min) of the heap. 
    
    @ params:   'heap' - A heap data structure created by HeapCreate.

    @ return value: Top (min) of the heap. 

    @ errors: If the heap is empty, the behaviour is undefined. 
    
    @ time complexity:  O(log(n)).
    
    @ space complexity: O(n).
-----------------------------------------------------------------------------*/
void* HeapPop(heap_t* heap);

/* -----------------------------------------------------------------------------
    @ description:      Checks the maximum element in the heap.          
    
    @ params:           heap - The heap data structure.   
    
    @ return value:     Returns a pointer to the maximum element.

    @ errors:           If heap is empty, behaviour is undefined.
    
    @ time complexity:  O(1).
    
    @ space complexity: O(1).
-----------------------------------------------------------------------------*/
void* HeapPeek(const heap_t* heap);

/* -----------------------------------------------------------------------------
    @ description: Returns the number of elements in the heap.         
    
    @ params: heap: The heap data structure.              

    @return value:  Returns the number of elements in the heap.

    @ errors: none.           
    
    @ time complexity: O(1)
    
    @ space complexity: O(1)
-----------------------------------------------------------------------------*/
size_t HeapSize(const heap_t* heap);

/* -----------------------------------------------------------------------------
    @ description: Checks whether the heap is empty or not.
    
    @ params: A pointer to the heap.
    
    @ return value: If the heap is empty -> returns 1.
                    Else -> returns 0.
    @ errors: None.
    
    @ time complexity: O(1).
    
    @ space complexity: O(1).
-----------------------------------------------------------------------------*/
int HeapIsEmpty(const heap_t* heap);

/* -----------------------------------------------------------------------------
    @ description: remove data from heap.
    
    @ params: a heap, a matching function, parameter holding data that will match
    the data to remove.

    @ return value: if data to remove was found => data found. else => NULL
    
    @ errors: none.
    
    @ time complexity:  O(n).
    
    @ space complexity: O(1).
-----------------------------------------------------------------------------*/
void* HeapRemove(heap_t* heap, int (*is_match)(const void* data
                                    , const void* param), const void* param);

#endif /*__HEAP_H__*/