#include <assert.h>
#include <stdlib.h>

#include "stack.h"

#define AMOUNT_OF_STRUCT_MEMBERS 3

#define UNUSED(x) (void)(x)

struct stack
{
	size_t capacity;
	size_t amount_of_elements;
	void **top;
};

stack_ptr_t StackCreate(size_t size_of_element, size_t capacity)
{	
	stack_ptr_t stack_ptr = NULL;
	
	assert(0 < size_of_element);
	assert(0 < capacity);
	
	stack_ptr = (stack_ptr_t)malloc((capacity + 
									AMOUNT_OF_STRUCT_MEMBERS) * sizeof(size_t));
	if (NULL == stack_ptr)
	{
		return NULL; 														
	}
	
	stack_ptr->capacity = capacity;
	stack_ptr->amount_of_elements = 0;
	stack_ptr->top = (void **)(stack_ptr + 1);
	
	UNUSED(size_of_element);
	return stack_ptr;
}

void StackDestroy(stack_ptr_t stack)
{
	assert(NULL != stack);
	
	free(stack);
}

void *StackPop(stack_ptr_t stack)
{	
	void *pop_element = NULL;
	
	assert(NULL != stack);
	
	if (!stack->amount_of_elements)
	{
		return NULL;
	}
	
	pop_element = *stack->top;
	
	--stack->top;
	--stack->amount_of_elements;
	
	if (!stack->amount_of_elements)
	{
		stack->top = (void **)(stack + 1);
	}
	
	return pop_element; 
}

int StackPush(stack_ptr_t stack, const void *element)
{
	assert(NULL != stack);
	
	if (stack->capacity == stack->amount_of_elements)
	{
		return 1;
	}
	
	if (stack->amount_of_elements)
	{
		++stack->top;	
	}
	
	*stack->top = (void *)element;
	++stack->amount_of_elements;
	
	return 0;
}

void *StackPeek(stack_ptr_t stack)
{
	assert(NULL != stack);
	
	return (stack->amount_of_elements ? *stack->top : NULL);
}

size_t StackSize(stack_ptr_t stack)
{
	assert(NULL != stack);
	
	return stack->amount_of_elements;
}

int StackIsEmpty(stack_ptr_t stack)
{	
	assert(NULL != stack);
		
	return !stack->amount_of_elements;
}

size_t StackCapacity(stack_ptr_t stack)
{
	assert(NULL != stack);
	
	return stack->capacity;
}
