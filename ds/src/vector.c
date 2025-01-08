#include <stdlib.h> /* malloc */
#include <assert.h> 
#include <string.h> /* memcpy */

#include "vector.h"

#define GROWTH_FACTOR 2

struct vector
{
	size_t size_of_element;
	size_t num_of_elements;
	size_t capacity;
	void *data;
};

vector_ptr_t VectorCreate(size_t size_of_element, size_t initial_capacity)
{
	vector_ptr_t vector_ptr = NULL;
	
	vector_ptr = (vector_ptr_t) malloc(sizeof(struct vector));
	if (NULL == vector_ptr)
	{
		return NULL; 														
	}									
		
	vector_ptr->size_of_element = size_of_element;
	vector_ptr->num_of_elements = 0;
	vector_ptr->capacity = initial_capacity;
	
	vector_ptr->data = (void *)malloc(size_of_element * initial_capacity);		
	if (NULL == vector_ptr->data)
	{
		free(vector_ptr);
		return NULL; 														
	}			
	
	return vector_ptr;						
}

void VectorDestroy(vector_ptr_t vector_ptr)
{
	assert(NULL != vector_ptr);
	
	free(vector_ptr->data);
	vector_ptr->data = NULL;
	
	free(vector_ptr);
	vector_ptr = NULL;	
}

void VectorAssign(vector_ptr_t vector_ptr, size_t index, void *value)
{
	assert(NULL != vector_ptr);
	assert(NULL != value);
	assert(vector_ptr->num_of_elements > index);
	
	memcpy((char *)vector_ptr->data + (index * vector_ptr->size_of_element)
										, value, vector_ptr->size_of_element);	
}

int VectorAppend(vector_ptr_t vector_ptr, void *value)
{
	void *tmp_data = NULL;
	char *dest = NULL;
	
	assert(NULL != vector_ptr);																																
	assert(NULL != value);
	
	if (vector_ptr->num_of_elements == vector_ptr->capacity)
	{
		tmp_data = (void *)realloc(vector_ptr->data, 
		(GROWTH_FACTOR * vector_ptr->capacity * vector_ptr->size_of_element));
		if (NULL == tmp_data)
		{
			return 1;
		}
		
		vector_ptr->data = tmp_data;
		vector_ptr->capacity *= GROWTH_FACTOR;
	}
	
	dest = (char *)vector_ptr->data 
				+ (vector_ptr->num_of_elements * vector_ptr->size_of_element);
	
	memcpy(dest, value, vector_ptr->size_of_element);

	++vector_ptr->num_of_elements;
	
	return 0;
}

int VectorPop(vector_ptr_t vector_ptr, void *value_dest)
{
	void *tmp_data = NULL;
	char *src = NULL;

	assert(NULL != vector_ptr);
	assert(NULL != value_dest);
	
	if (0 == vector_ptr->num_of_elements)
	{
		return 1;
	}
	
	src = (char *)vector_ptr->data + (vector_ptr->num_of_elements - 1) 
												* vector_ptr->size_of_element;
	
	memcpy(value_dest, src, vector_ptr->size_of_element);
			
	--vector_ptr->num_of_elements;

	if (vector_ptr->capacity / GROWTH_FACTOR >  vector_ptr->num_of_elements)
	{										  
		tmp_data = (void *) realloc(vector_ptr->data 
											, (vector_ptr->size_of_element 
									* (vector_ptr->capacity / GROWTH_FACTOR)));
	
		if (NULL != tmp_data)
		{
			vector_ptr->data = tmp_data;
			vector_ptr->capacity /= GROWTH_FACTOR;								
		}			
	}
	
	return 0;
}

void VectorGetVal(vector_ptr_t vector_ptr, void *value_dest, size_t index)
{
	char *src = NULL;
	
	assert(NULL != vector_ptr);
	assert(NULL != value_dest);
	assert(index < vector_ptr->num_of_elements);
	
	src = (char *)vector_ptr->data + index * vector_ptr->size_of_element;
	memcpy(value_dest, src, vector_ptr->size_of_element);
}

int VectorIsEmpty(vector_ptr_t vector_ptr)
{	
	assert(NULL != vector_ptr);
	
	return !vector_ptr->num_of_elements;
}

size_t VectorSize(vector_ptr_t vector_ptr)
{
	assert(NULL != vector_ptr);
	
	return vector_ptr->num_of_elements;
}
