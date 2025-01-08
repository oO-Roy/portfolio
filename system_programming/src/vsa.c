/* 
	Developer: Roy Steiner.
	File: Fixed size allocator.
	Reviewer: Amos Duek.
	Date: 21.07.2024
	Status: Approved.
*/

#include <assert.h>

#include "vsa.h" /* typedef vsa_t */

#define STRCT_SIZE sizeof(vsa_t)
#define WORD_SIZE (__WORDSIZE >> 3)
#define OCCUPIED 0x1

struct vsa
{
	size_t size;
};

static size_t BlockAlignment(size_t block_size)
{
	return ((block_size + WORD_SIZE - 1) & ~(WORD_SIZE - 1));
}

static int IsChunkFree(const vsa_t *chunk)
{
	assert(NULL != chunk);
	
	/* returns 1 when free */
	return !(chunk->size & 0x1);
}

static vsa_t *StartOfPool(vsa_t *vsa)
{
	return (vsa_t *)((char *)vsa + STRCT_SIZE);
}

static void *EndOfPool(vsa_t *vsa)
{
	return (void *)((char *)vsa + vsa->size);
}

static vsa_t *GetNextChunk(vsa_t *runner)
{
	return (vsa_t *)((char *)runner + (runner->size & ~1) + STRCT_SIZE);
}

static void VSADefragmentation(vsa_t *vsa)
{
	vsa_t *runner = NULL;
	vsa_t *next_block = NULL;
	void *end_of_pool = NULL;
	
	assert(NULL != vsa);
	
	runner = StartOfPool(vsa);
	end_of_pool = EndOfPool(vsa);
	
	while ((vsa_t *)end_of_pool != runner)
	{
		if (IsChunkFree(runner))
		{
			next_block = GetNextChunk(runner);
			
			while (((vsa_t *)end_of_pool != next_block) 
								&& (IsChunkFree(next_block)))
			{
				runner->size += next_block->size + STRCT_SIZE;
				next_block = GetNextChunk(next_block);
			}
		}
		
		runner = GetNextChunk(runner);	
	}

}

vsa_t *VSACreate(void *base, size_t pool_size)
{		
	vsa_t *vsa = NULL;
	vsa_t *block = NULL;
	
	assert(NULL != base);

	vsa = (vsa_t *)base;	
	vsa->size = pool_size;

	block = StartOfPool(vsa);	
	block->size = (pool_size - STRCT_SIZE - STRCT_SIZE);
	
	return vsa;
}

void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	vsa_t *runner = NULL;
	vsa_t *next_header = NULL;
	void *end_of_pool = NULL;
	
	assert(NULL != vsa);
	
	end_of_pool = EndOfPool(vsa);
	runner = StartOfPool(vsa);
	block_size = BlockAlignment(block_size);	
	VSADefragmentation(vsa);
		
	while ((vsa_t *)end_of_pool != runner)
	{
		if ((IsChunkFree(runner)) && (block_size <= runner->size))												  
		{
			if (block_size <= runner->size - STRCT_SIZE)						
			{
				next_header = (vsa_t *)((char *)runner 
										+ STRCT_SIZE + block_size);
				next_header->size = runner->size - block_size - STRCT_SIZE;
			}
				
			runner->size = block_size + OCCUPIED;
				
			return (void *)((char *)runner + STRCT_SIZE);
		}
		
		runner = GetNextChunk(runner);
	}
	
	return NULL;
}

void VSAFree(void *block_to_free)
{
	vsa_t *block = NULL;
	
	if (NULL != block_to_free)
	{
		block = (vsa_t *)((char *)block_to_free - STRCT_SIZE);
		block->size -= OCCUPIED;
	}
}

size_t VSALargestChunk(vsa_t *vsa)
{
	vsa_t *runner = NULL;
	void *end_of_pool = NULL;
	
	size_t largest_size = 0;
	
	assert(NULL != vsa);

	runner = StartOfPool(vsa);
	end_of_pool = EndOfPool(vsa);
	
	VSADefragmentation(vsa);
	
	while ((vsa_t *)end_of_pool != runner)
	{
		if (IsChunkFree(runner))												
		{
			if (runner->size > largest_size)					
			{
				largest_size = runner->size;
			}
		}
		
		runner = GetNextChunk(runner); 
	}
	
	return largest_size;
}
