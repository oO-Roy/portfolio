#ifndef __FSA_H__
#define __FSA_H__

#include <stddef.h>

typedef struct fsa fsa_t;

/*
    @ description: calculate minimal memory size (in bytes) needed to implement an allocator containing 'block_size' and 'num_of_blocks'.
    @ params: block_size - the required block size (in bytes),
              num_of_blocks - the number of required blocks. 
    @ return value: suggested memory size (in bytes).
    @ errors: none.
    @ time complexity: O(1).
    @ space complexity: O(1).
*/
size_t FSASuggestSize(size_t block_size, size_t num_of_blocks);

/*
    @ description: create an allocator object.
    @ params: memory_base - a pointer to the base of memory
              for the allocator (needs to be aligned to word_size).
              memory_size - the size of memory provided to the allocator.
              block_size - the required block size,
    @ return value: a pointer to an alocator object,
                    or NULL if not enough memory provided to allocator.
    @ errors: not enough memory provided to allocator.
    @ time complexity: O(num_of_blocks).
    @ space complexity: O(1).
*/
fsa_t *FSACreate(void *memory_base, size_t memory_size, size_t block_size);

/*
    @ description: returns a pointer to a block of memory with allocator's size_of_block.
    @ params: allocator - a pointer to an allocator object. 
    @ return value: a pointer to a block of memory with size_of_block,
                    or NULL if no memory is available.
    @ errors: NULL if no memory is available.
    @ time complexity: O(1).
    @ space complexity: O(1).
*/
void *FSAAllocate(fsa_t *allocator);

/*
    @ description: frees a block of memory.
    @ params: allocator - a pointer to an allocator object. ,
              block - a block recived by the FSAAllocate() function.
    @ return value: none.
    @ errors: trying to free an address that is not the beginning of a block is undefined.
    @ time complexity: O(1).
    @ space complexity: O(1).
*/
void FSAFree(fsa_t *allocator, void *block);

/*
    @ description: returns the number of free blocks in the allocator.
    @ params: allocator - a pointer to an allocator object. 
    @ return value: the number of free blocks in allocator.
    @ errors: none.
    @ time complexity: O(num_of_blocks).
    @ space complexity: O(1)
*/
size_t FSACountFree(const fsa_t *allocator);

#endif /* __FSA_H__ */
