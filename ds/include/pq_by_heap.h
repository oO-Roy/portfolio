#ifndef __PQ_BY_HEAP_H__
#define __PQ_BY_HEAP_H__

#include <stddef.h> /*size_t*/

typedef struct pqueue pq_t;

/*
    @ description: Function pointer type for a comparison function used to order elements in a periority queue.
                   The comparison function returns an integer is less than, equal to, or greater than zero.
    @ params: data: Pointer to the data of the first element to compare.
                    param: Pointer to the data of the second element to compare.
    @ return value: Positive integer if 'data' is greater then 'param'.
                    Zero if 'data' is considered equal to 'param'.
                    Negative integer if 'data' is less then 'param'.
*/
typedef int (*pq_comp_func) (const void *data1, const void *data2);

/*
    @ description: Function pointer type used to determine if an element 
    			contains certain data in the priority queue. 
    @ params:
        - data: A pointer to the data of the element to check the condition.
        - param: A pointer the data to be searched.
    @ return value:
        - 1 (true) if the element matches the condition.
        - Zero (false) otherwise. 
*/
typedef int (*pq_is_match_func) (const void *data, const void *param);

/*
    @ description: the function shall create an empty priority queue. 
    @ params: pq_comp_func comp - A pointer to a comparison function
    		  that defines the perioritiy of the queue elements.
    @ return values: pq_t* - A pointer to the created queue.
    @ errors: Returns NULL if memory allocation fails.
    @ time complexity: O(1). 
*/
pq_t *PQCreate(pq_comp_func comp);

/*
    @ description: the function will destroy the priority queue.
    @ params: pq_t *pq.
    @ return values: none.
    @ errors: none.
    @ time complexity: O(n).
*/
void PQDestroy(pq_t *pq);

/*
    @ description: the function shall insert an element on the correct location.
    @ params: pq_t *pq and const void *data.
    @ return values: 0 on success and 1 if failed
    @ errors: returns 1 if memory allocation failed
    @ time complexity: O(n).
*/
int PQEnqueue(pq_t *pq, const void *data);

/*
    @ description: takes out the most preoritized node in list
    @ params: pointer to DLlist
    @ return values: void pointer
    @ errors: in case of empty list returns NULL
    @ time complexity: O(1)
*/
void *PQDequeue(pq_t *pq);

/*
    @ description: the function shall present the value of the end element.
    @ params: const pq_t *pq.
    @ return values: a pointer to the element with the highest priority. 
    @ errors: none.
    @ time complexity: O(1).
*/
void *PQPeek(const pq_t *pq);

/*
    @ description: clear the Queue from all the elements.
    @ params: pq_t *pq.
    @ return values: None.
    @ errors: none.
    @ time complexity: O(n).
*/
void PQClear(pq_t *pq);

/*
    @ description: the function checks for the size of the priority queue.
    @ params: const pq_t *pq.
    @ return value: returns the size of the given priority queue
    @ errors: no errors
    @ time complexity: O(n)
*/
size_t PQSize(const pq_t *pq);

/*
    @ description: check if a priority queue is empty.
    @ params: const pq_t *pq.
    @ return values: if priority queue is empty - returns 1, else - returns 0.
    @ errors: none.
    @ time complexity: O(1).
*/
int PQIsEmpty(const pq_t *pq);

/*
    @ description: the function shall erase a specific element which depands on the found data.
    @ params: pq_t *pq, pointer to is_match function, data.
            - pq -  pointer to a priority queue that the data will be erased from.
            - is_match - function pointer to pq_is_match_func. See its description above.
            - data - void pointer to a data that is going to be deleted.
    @ return values: the data that was deleted represented in void pointer.
    @ errors: 
            - when data is not on the queue, NULL will be returned.
    @ time complexity: O(n)
*/
void *PQErase(pq_t *pq, pq_is_match_func is_match, void *data);

#endif /*__PQ_BY_HEAP_H__*/
