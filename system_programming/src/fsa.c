/* 
	Developer: Roy Steiner.
	File: Fixed size allocator.
	Reviewer: Mentor - Omer.
	Date: 18.07.2024
	Status: Approved.
*/

#include <assert.h> /* assert */

#include "fsa.h" /* typedef fsa_t, size_t */

#define FSA_END 0
#define WORD_SIZE (__WORDSIZE >> 3)

struct fsa
{
	size_t offset_next;
};

static size_t BlockAlignment(size_t block_size)
{
	return ((block_size + WORD_SIZE - 1) & ~(WORD_SIZE - 1));
}

size_t FSASuggestSize(size_t block_size, size_t num_of_blocks) 
{
	block_size = BlockAlignment(block_size);
	
	return (block_size * num_of_blocks + sizeof(fsa_t));
}

fsa_t *FSACreate(void *memory_base, size_t memory_size, size_t block_size)
{
	fsa_t *allocator = NULL;
	size_t *block_runner = NULL;
	size_t num_of_blocks = 0;
	size_t i = 1;
	size_t offset = sizeof(fsa_t);
	
	assert(NULL != memory_base);
	
	allocator = (fsa_t *)memory_base;
	block_size = BlockAlignment(block_size);
	
	num_of_blocks = ((memory_size - sizeof(fsa_t)) / block_size);
	
	allocator->offset_next = offset;
	block_runner = (size_t *)((char *)allocator 
						+ allocator->offset_next);
	
	for (; i < num_of_blocks ; ++i)
	{
		offset += block_size;		
		*block_runner = offset;
		block_runner = (size_t *)((char *)allocator + offset);
	}
	
	*block_runner = FSA_END;
		
	return allocator;
}

void *FSAAllocate(fsa_t *allocator)
{
	void *block = NULL;
	
	assert(NULL != allocator);

	if (FSA_END == allocator->offset_next)
	{
		return NULL;
	}
	
	block = (char *)allocator + allocator->offset_next;
	allocator->offset_next =
			*(size_t *)((char *)allocator + allocator->offset_next);

	return block; 
}

void FSAFree(fsa_t *allocator, void *block)
{
	assert(NULL != allocator);

	if (NULL == block)
	{
		return;
	}

	*(size_t *)block = allocator->offset_next;
	allocator->offset_next = (size_t)((char *)block - (char *)allocator);
}

size_t FSACountFree(const fsa_t *allocator)
{
	size_t block_runner = 0;
	size_t count = 0;
	
	assert(NULL != allocator);
	
	block_runner = allocator->offset_next;
	
	for ( ; FSA_END != block_runner 
				; block_runner = *(size_t *)((char *)allocator + block_runner))
	{
		++count;
	}
	
	return count;
}
