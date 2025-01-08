#ifndef __MT_COUNTINGSORT_H__
#define __MT_COUNTINGSORT_H__

#include <stddef.h>     /* for size_t type */

/*
    @Returns:       0 on SUCCESS, 1 on ALLOC_FAIL, 2 on SYS_FAIL
                    ,3 on ARGS_INVALID.
*/
int MTCountingSort(char* arr, size_t size, size_t num_threads);

#endif /*__MT_COUNTINGSORT_H__*/