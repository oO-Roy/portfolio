/* 
	Developer: Roy Steiner.
	File: Sorts.
	Reviewer: Yoni. 
	Date: 28/07/24
	Status: Approved.
*/

#include <stdlib.h> /* malloc & free */
#include <assert.h>

#include "sorts.h" /* size_t */

static void Swap(int *a, int *b)
{
	if (a == b)
	{
		return;
	}
	
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

static void HeapifyDown(int arr[], size_t size, size_t index)
{
	size_t left = 2 * index + 1;
	size_t right = 2 * index + 2;
	size_t max = index;

	if (left < size && arr[left] > arr[index])
	{
		max = left;
	}
	
	else
	{
		max = index;
	}

	if (right < size && arr[right] > arr[max])
	{
		max = right;
	}

	if (max != index)
	{
		Swap(&arr[index], &arr[max]);
		HeapifyDown(arr, size, max);
	}
}

static void BuildMaxHeap(int arr[], size_t size)
{
	int i = ((size / 2) - 1);

	for ( ; i >= 0 ; --i)
	{
		HeapifyDown(arr, size, i);
	}
}

void HeapSort(int arr[], size_t size)
{
	int i = (size - 1);
	
	BuildMaxHeap(arr, size);

	for ( ; i > 0 ; --i)
	{
		Swap(&arr[0], &arr[i]);
		HeapifyDown(arr, i, 0);
	}
}

static void SwapBySize(void *a, void *b, size_t size_element)
{
	size_t i = 0;
	
	unsigned char *p_a = a;
	unsigned char *p_b = b;
	unsigned char tmp = 0; 
	
	if (a == b) 
	{
		return;
	}
	
	for ( ; i < size_element ; ++i)
	{
		tmp = *(p_a + i);
		*(p_a + i) = *(p_b + i);
		*(p_b + i) = tmp;
	}
}

static int Partition(void *arr, size_t num_elements, size_t size_element
													, qsort_comp_func comp)
{
	size_t i = 0;
	int swap_marker = -1;
	void *swap_idx = NULL;
	void *current_idx = NULL;
	void *pivot = ((char *)arr + (num_elements - 1) * size_element);

	for (i = 0 ; i < num_elements ; ++i)
	{
		current_idx = ((char *)arr + i * size_element);

		if (0 <= comp(pivot, current_idx)) 
		{
			++swap_marker;
			swap_idx = ((char *)arr + swap_marker * size_element);
			
			if (0 >= comp(current_idx, swap_idx))
			{
				 SwapBySize(current_idx, swap_idx, size_element);
			}
		}
	}	
	
	return swap_marker;
}

void QSort(void *arr, size_t num_elements, size_t size_element
											, qsort_comp_func comp)
{
	int pivot_location = 0;
	
	assert(NULL != arr);
	assert(NULL != comp);
	
	if (0 < num_elements)
	{
		pivot_location = Partition(arr, num_elements, size_element, comp);

		QSort(arr, (size_t)pivot_location, size_element, comp);
		QSort(((char *)arr + (pivot_location + 1) * size_element)
									, num_elements - (pivot_location + 1)
														, size_element, comp);
	}
}

static void Merge(int arr[], int l_arr[], int r_arr[], size_t size
			, size_t l_size, size_t r_size)
{
	size_t i = 0;
	size_t l_idx = 0;
	size_t r_idx = 0;
	
	for (; (i < size) && (l_idx < l_size) && (r_idx < r_size) ; ++i)
	{
		if (l_arr[l_idx] < r_arr[r_idx])
		{
			arr[i] = l_arr[l_idx++];
		}
		
		else
		{
			arr[i] = r_arr[r_idx++];
		} 
	}
	
	for ( ; l_idx < l_size ; ++l_idx, ++i)
	{
		arr[i] = l_arr[l_idx];
	}
	
	for ( ; r_idx < r_size ; ++r_idx, ++i)
	{
		arr[i] = r_arr[r_idx];
	}
}

static void FillArr(int dest[], int src[], size_t size)
{
	size_t i = 0;
	
	for (; i < size ; ++i)
	{
		dest[i] = src[i];
	}
}

int MergeSort(int arr[], size_t num_elements)
{
	int *arr_right = NULL;
	int *arr_left = NULL;
	size_t size_l = (num_elements >> 1);
	size_t size_r = size_l + (num_elements & 0x1);
	
	assert(NULL != arr);
	
	if (1 < num_elements)
	{
		arr_right = (int *)malloc(sizeof(int) * size_r);
		if (NULL == arr_right)
		{
			return -1;
		}
		
		arr_left = (int *)malloc(sizeof(int) * size_l);
		if (NULL == arr_left)
		{
			free(arr_right);
			return -1;
		}
		
		FillArr(arr_left, arr, size_l);
		FillArr(arr_right, arr + size_l, size_r);
		
		MergeSort(arr_left, size_l);
		MergeSort(arr_right, size_r);
		
		Merge(arr, arr_left, arr_right, num_elements, size_l, size_r);
		
		free(arr_right);	
		free(arr_left);
	}
	
	return 0;
}

void RadixSort(int arr[], size_t size)
{
	size_t i = 0;
	int j = 0;
	int max_val = arr[0];
	int exp = 1;
	int pos = 0;
	int *radix_arr[10];
	size_t radix_index = 0;
	int counts[10] = {0};
	
	for (i = 0 ; i < size ; ++i)
	{
		if (max_val < arr[i])
		{
			max_val = arr[i];
		}
	}
	
	for (i = 0 ; i < 10 ; ++i)
	{
		radix_arr[i] = (int *)malloc(sizeof(int) * size);
	}
	
	while (0 < (max_val / exp))
	{
		for (i = 0 ; i < size ; ++i)
		{
			radix_index = (arr[i] / exp) % 10;
			radix_arr[radix_index][counts[radix_index]] = arr[i];
			++counts[radix_index];
		} 
		
		pos = 0;
	
		for (i = 0 ; i < 10 ; ++i)
		{
			for (j = 0 ; j < counts[i] ; ++j)
			{
				arr[pos] = radix_arr[i][j];
				++pos;
			}
			
			counts[i] = 0;
		}
		
		exp *= 10;
	}
	
	for (i = 0 ; i < 10 ; ++i)
	{
		free(radix_arr[i]);
	}
}

void CountingSort(int arr[], size_t size)
{
	size_t i = 0;
	size_t j = 0;
	int *LUT = NULL;
	int max_val = arr[0];
	int min_val = arr[0];
	int offset = 0;
	size_t LUT_size = 0;
	
	for (i = 0 ; i < size ; ++i)
	{
		if (min_val > arr[i])
		{
			min_val = arr[i];
		}
		
		if (max_val < arr[i])
		{
			max_val = arr[i];
		}
	}	
	
	LUT_size = (min_val < 0 ) ? (max_val + (-min_val) + 1) : (max_val + 1);
	
	LUT = (int *)calloc(LUT_size, sizeof(int));
	
	if (min_val < 0)
	{
		offset = -min_val;
	}
	
	for (i = 0 ; i < size ; ++i)
	{
		++LUT[arr[i] + offset];
	}
	
	j = 0;
	
	for (i = 0 ; i < size ; ++j)
	{
		for (; LUT[j] > 0 ; ++i, --LUT[j])
		{
			arr[i] = j - offset;
		}
	}
	
	free(LUT);
}

void InsertionSort(int arr[], size_t size)
{
	size_t i = 0;
	int j = 0;
	int tmp = 0;
	
	for (i = 1 ; i < size; ++i)
	{
		tmp = arr[i];
		
		for (j = (i - 1) ; (j >= 0) && (tmp < arr[j]) ; --j)					
		{
			arr[j + 1] = arr[j];									
		}
		
		arr[j + 1] = tmp;
	}	
}


void SelectionSort(int arr[], size_t size)
{
	size_t i = 0;
	size_t j = 0;
	size_t idx_smallest = 0;

	
	for (i = 0 ; i < (size - 1); ++i)											/* include optimization for condition (size - 1) */
	{
		for (j = (i + 1), idx_smallest = i ; j < size ; ++j)					/* include optimization for initialization (i + 1) */
		{
			if (arr[j] < arr[idx_smallest])
			{
				idx_smallest = j;										
			}
		}
		
		if (i != idx_smallest)													/* optimization for unnecessary cycles */
		{
			Swap((arr + i), (arr + idx_smallest));
		}
	}	
}

void BubbleSort(int arr[], size_t size)
{
	size_t i = 0;
	size_t j = 0;
	int is_swaped = 0;
	
	for (i = (size - 1)	; i > 0 ; --i) 		 									/* changes the definition of end */
	{
		is_swaped = 0;															/* clear is_swaped for next round */
		
		for (j = 1 ; j <= i ; ++j) 												/* put the biggest num at the end */
		{
			if (arr[j - 1] > arr[j])											/* swap if needed */
			{
				Swap((arr + j), (arr + j - 1));
				is_swaped = 1;												
			}
		}
	
		if (!is_swaped)															/* checks if no swapping happened so we can skip loops */
		{
			break;
		}
	}
}
