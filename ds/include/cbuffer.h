#ifndef __C_BUFFER_H__
#define __C_BUFFER_H__

#include <stddef.h>     /* for size_t type */
#include <unistd.h> 	/* for ssize_t type */

typedef struct cbuffer cbuff_t;

/*
    @ description: cbuffer constructor.
    @ params:  buffer capacity.
    @ return value: pointer to new cbuffer.
    @ errors: if memory allocation fail - return NULL.
    @ complexity: O(1).
*/
cbuff_t *CBuffCreate(size_t capacity);

/*
    @ description: buffer memory deallocation.
    @ params: buffer pointer.
    @ return value: none.
    @ errors: none.    
    @ complexity: O(1)
*/
void CBuffDestroy(cbuff_t *buffer);

/*
    @ description: writes n bytes from src to buffer. 
    @ params: a pointer to the buffer to write to, a pointer to src to be written from, number of bytes to write       
    @ return value: the number of bytes that were written
    @ errors: in Debug version - if magic_num (struct id) does not match - returns -1
            in Release version - no error are returned 
    @ complexity: O(n)   
*/
ssize_t CBuffWrite(cbuff_t *buffer, const void *src, size_t n);

/*
    @ description: reads n bytes from buffer to dest.
    @ params: pointer to cbuff, pointer to dest buffer, number of bytes to read.
    @ return value: the number of bytes actually read.
    @ errors: in Debug version - if magic_num (struct id) does not match - returns -1
            in Release version - no error are returned
    @ complexity: O(n)  
*/
ssize_t CBuffRead(cbuff_t *buffer, void *dest, size_t n); 

/*
    @ description: checks if buffer is empty;
    @ params: pointer on buffer's head;       
    @ return value: 1 in case its empty, 0 is not;
    @ errors: invalid pointer;      
    @ complexity: O(1);  
*/
int CBuffIsEmpty(const cbuff_t *buffer);

/*
    @ description: returns the capacity of the buffer;
    @ params: pointer on buffer's head;       
    @ return value: returns the buffers capacity;
    @ errors: undefined behavior if the buffer is NULL; 
*/
size_t CBuffGetCapacity(const cbuff_t *buffer);

/*
    @ description: check the amount of free space in the buffer.
    @ params: pointer to buffer.
    @ return value: size of free space. 
    @ errors: no errors.
    @ complexity: O(1).
*/
size_t CBuffFreeSpace(const cbuff_t *buffer);

#endif /* __C_BUFFER_H__ */
