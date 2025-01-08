#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h> /* size_t */

typedef struct vsa vsa_t;

/*
    @ description: creates an allocator object.
    @ params: pointer to memory pool and the size of the given memory.
    @ return value: a pointer to the alloctor.
    @ errors: none.
    @ time complexity: O(1).
    @ space complexity: O(1).
*/
vsa_t *VSACreate(void *base, size_t total_size);

/*
    @ description: Allocates a memory block of `block_size` bytes from the given VSA.
    @ params: 
        - vsa: Pointer to the variable size allocator (vsa_t).
        - block_size: Size in bytes of the memory block to allocate.
    @ return value: Pointer to the allocated memory block, or NULL if allocation fails.
    @ errors: Returns NULL if no suitable block is available.
    @ time complexity: o(n).
    @ space complexity: O(1).
*/
void *VSAAlloc(vsa_t *vsa, size_t block_size);

/*
    @ description: free an allocated block.
    @ params: pointer to memory block to free.
    @ return value: none.
    @ errors: 
        - double free (freeing a free block) => undefined behavior.
    @ time complexity: O(1).
    @ space complexity: O(1).
*/
void VSAFree(void *block_to_free);

/*
    @ description: returns the largest chunk available to allocate.
    @ params: pointer to vsa.
    @ return value: size of largest chunk available to allocate.
    @ errors: none
    @ time complexity: O(n).
    @ space complexity: O(1).
*/
size_t VSALargestChunk(vsa_t *vsa);

#endif /* __VSA_H__ */
