#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h> /* for size_t */

typedef struct stack *stack_ptr_t;

/* -------------------------------------------------------------------------------------------------
    Create the stack with the capacity and sizeof elements */
stack_ptr_t StackCreate(size_t size_of_element, size_t capacity);

/* Delete the stack */
void StackDestroy (stack_ptr_t stack);

/* -------------------------------------------------------------------------------------------------
   Get the element at the top of the stack by saving it to pop_to_this argument and removing it 
   from the stack. When return 1 - the stack was empty ( so pop_to_this will be NULL), when return 
   0 - the action succeed */
void *StackPop(stack_ptr_t stack);

/* -------------------------------------------------------------------------------------------------
    Save an element on top of stack, return 1 - the stack was full already and return 0 - the
    action succeed */
int StackPush(stack_ptr_t stack, const void *element);

/* -------------------------------------------------------------------------------------------------
    Get the element at the top of the stack ( this element stays in the stack. ) */
void *StackPeek(stack_ptr_t stack);

/* ------------------------------------------------------------------------------------------------- 
    Return current number of elements inside the stack */
size_t StackSize(stack_ptr_t stack);

/* -------------------------------------------------------------------------------------------------
    Return 1 - if stack is empty, return 0 - if stack is not empty */
int StackIsEmpty(stack_ptr_t stack);

/* -------------------------------------------------------------------------------------------------
    Return the amount of total elements that can be pushed to the stack */
size_t StackCapacity(stack_ptr_t stack);


#endif /* __STACK_H__ */



