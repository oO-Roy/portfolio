#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h> /* for size_t */

typedef struct vector *vector_ptr_t;

/* -----------------------------------------------------------------------------
    Create a vector with initial capacity and size of its elements 
*/
vector_ptr_t VectorCreate(size_t size_of_element, size_t initial_capacity);

/* -----------------------------------------------------------------------------
    Delete a vector 
*/
void VectorDestroy(vector_ptr_t vector_ptr);

/* -----------------------------------------------------------------------------
    Add a value to the vector at specific index. Upon success - return 0, upon 
    failure - return 1. Index must be in range of already existing elements of 
    the vector, otherwise the behavior is undefined  
*/
void VectorAssign(vector_ptr_t vector_ptr, size_t index, void *value);

/* -----------------------------------------------------------------------------
    Add a value to the vector at the end of the vector. Upon success - return 0,
    upon  failure - return 1 ( Possible failures: allocation failure ) 
*/
int VectorAppend(vector_ptr_t vector_ptr, void *value);

/* -----------------------------------------------------------------------------
    Delete last element and save its value into value_dest. Upon success - 
    return 0, upon failure - return 1 ( Possible failures: allocation failure )
*/
int VectorPop(vector_ptr_t vector_ptr, void *value_dest);

/* -----------------------------------------------------------------------------
    Get a value from a specific index and save it in value_dest. If success - 
    return 0, if failed - return 1
*/
void VectorGetVal(vector_ptr_t vector_ptr, void *value_dest, size_t index);

/* -----------------------------------------------------------------------------
    Check if the vector is empty. Return 1 - if it is, return 0 - if it is not 
*/
int VectorIsEmpty(vector_ptr_t vector_ptr);

/* -----------------------------------------------------------------------------
    Get number of elements in the vector 
*/
size_t VectorSize(vector_ptr_t vector_ptr);

#endif /* __VECTOR_H__ */

