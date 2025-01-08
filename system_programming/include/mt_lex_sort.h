#ifndef __MT_LEX_SORT_H__
#define __MT_LEX_SORT_H__

#include <stddef.h> /* for size_t type */

int MTLexographicSort(const char* words[], size_t size, size_t num_threads);

#endif /*__MT_LEX_SORT_H__*/