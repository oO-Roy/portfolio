#include <stdlib.h> /* malloc & free */
#include <assert.h>

#include "cbuffer.h" /* typedef, size_t, ssize_t */
 
#define OFFSETOF(s, f) ((char *) s->f - (char *) s)	/* only for record */
#define MAGIC_NUM 0xC0DEBABEBABEC0DE

struct cbuffer
{
	#ifndef NDEBUG
		size_t magic_num;
	#endif
	size_t capacity;
	size_t num_of_bytes;
	size_t read;
	char data[1]; 
};

cbuff_t *CBuffCreate(size_t capacity)
{
	cbuff_t *buffer = (cbuff_t *) malloc(offsetof(cbuff_t, data) + capacity); 		
	if (NULL == buffer)
	{
		return NULL;
	}

	buffer->capacity = capacity;
	buffer->num_of_bytes = 0;
	buffer->read = 0;
	
	#ifndef NDEBUG
		buffer->magic_num = MAGIC_NUM;
	#endif

	return buffer;
}

void CBuffDestroy(cbuff_t *buffer)
{
	free(buffer); buffer = NULL;
} 

ssize_t CBuffWrite(cbuff_t *buffer, const void *src, size_t n)
{
	size_t bytes_written = 0;
	char *dest = (char *) buffer + offsetof(cbuff_t, data) 
						+ (buffer->num_of_bytes % buffer->capacity);
	char *s_runner = (char *) src;
	
	assert(NULL != buffer);
	assert(NULL != src);
	
	#ifndef NDEBUG
		if (MAGIC_NUM != buffer->magic_num)
		{
			return -1;
		}
	#endif
	
	while (n && CBuffFreeSpace(buffer)) 
	{					
		*dest = *s_runner;
		
		++buffer->num_of_bytes;
		++bytes_written;
        dest = (char *) buffer + offsetof(cbuff_t, data) 
						+ (buffer->num_of_bytes % buffer->capacity);
        ++s_runner;
        --n;
	}
	 
	return bytes_written;
}

ssize_t CBuffRead(cbuff_t *buffer, void *dest, size_t n)
{
	size_t bytes_read = 0;
	char *src = (char *) buffer + offsetof(cbuff_t, data) 
						+ (buffer->read % buffer->capacity);
	char *d_runner = (char *) dest;
	
	assert(NULL != buffer);
	assert(NULL != dest);
	
	#ifndef NDEBUG
		if (MAGIC_NUM != buffer->magic_num)
		{
			return -1;
		}
	#endif
	
	while (n && !CBuffIsEmpty(buffer))                    
	{			
		*d_runner = *src;
		
		++buffer->read;
		++bytes_read;
        ++d_runner;
        src = (char *) buffer + offsetof(cbuff_t, data) 
						+ (buffer->read % buffer->capacity);
        --n;
	}
	
	return bytes_read;
}

int CBuffIsEmpty(const cbuff_t *buffer)
{
	assert(NULL != buffer);

	return !((buffer->num_of_bytes) - (buffer->read));
}

size_t CBuffGetCapacity(const cbuff_t *buffer)
{
	assert(NULL != buffer);
	
	return buffer->capacity;
}

size_t CBuffFreeSpace(const cbuff_t *buffer)
{	
	assert(NULL != buffer);
	
	return (CBuffGetCapacity(buffer) - ((buffer->num_of_bytes) - (buffer->read)));
}

