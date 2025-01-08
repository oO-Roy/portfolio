#ifndef __SORTS_H__ 
#define __SORTS_H__ 

#include <stddef.h> /* size_t */


typedef int (*qsort_comp_func) (const void*, const void*);

/*--------------- Comparison sorts ---------------*/
void BubbleSort(int arr[], size_t size);
void SelectionSort(int arr[], size_t size);
void InsertionSort(int arr[], size_t size);
void HeapSort(int arr[], size_t size);

/*--------------- Linear sorts ---------------*/
void CountingSort(int arr[], size_t size);
void RadixSort(int arr[], size_t size);

/*--------------- Recursive sorts ---------------*/
int MergeSort(int arr[], size_t size);
void QSort(void *arr, size_t num_elements, size_t size_element
											, qsort_comp_func comp);

#endif /* __SORTS_H__ */
