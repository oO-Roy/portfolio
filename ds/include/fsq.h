#ifndef __FSQ_H__
#define __FSQ_H__

#include <stddef.h> /*size_t*/

/* NOTE: This is a thread-safe API for the user */

typedef struct fsq fsq_t;

fsq_t* FSQCreate(size_t capacity);

void FSQDestroy(fsq_t* queue);

int FSQEnqueue(fsq_t* queue, const void* data, size_t n);

void FSQDequeue(fsq_t* queue, void* data, size_t n);

#endif	/* __FSQ_H__ */
