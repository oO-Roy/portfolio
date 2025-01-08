#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h> /* size_t for size function*/

typedef struct queue queue_t;

/*
    @ description:  creates an empty queue data structure.
    @ params:       no parameters
    @ return value: returns pointer to new queue.
    @ errors:       returns NULL if memory allocation failed.
    @ complexity:   O(1)
*/
queue_t *QueueCreate(void);


/*
    @ description: QueueDestroy gets queue pointer and free memory allocated
                    for queue struct and its elemnts
    @ params: queue_t *queue 
    @ return values: void
    @ errors: no errors
    @ complexity: o(n)
*/

void QueueDestroy(queue_t *queue);

/*
    @ description: Enqueue adds new node to the tail, saving the dummy;
    @ params: queue pointer, data pointer;
    @ return values: 0 to seccess, 1 to fail;
    @ errors: malloc fail, not asserted pointers;
    @ complexity: O(1);
*/
int QueueEnqueue(queue_t *queue, const void *data);

/*
    @ description: remove node from the top of the list;
    @ params: queue pointer;
    @ return value: void;
    @ errors: dequeue on empty queue results in an undefined behavior;
    @ complexity: O(1);
*/
void QueueDequeue(queue_t *queue);

/*
    @ description: QueueSize function return the size of the queue.
    @ params: queue_t pointer
    @ return values: num (in size_t) of elemnts in the queue
    @ errors: no errors. return 0 if queue is empty
    @ complexity: O(n). n = num elements. 
*/
size_t QueueSize(const queue_t *queue);

/*
    @ description: QueueIsEmpty function checks if a queue is empty.
    @ params: queue_t pointer.
    @ return values: 1 if queue is empty, else 0.
    @ errors: no errors.
    @ complexity: 0(1).
*/
int QueueIsEmpty(const queue_t *queue);

/*
    @ description: QueuePeek fuction check 
    @ params: queue_t pointer.
    @ return values: 1 if queue is empty, else 0.
    @ errors: Peek to empty queue is undefind behavior.
    @ complexity: 0(1).
*/

void *QueuePeek(const queue_t *queue);
/*
    @ description: Appends src queue to dest queue, after function call src queue is empty.
    @ params: dest queue, src queue.
    @ return values: pointer to modified queue (queue_t *).
    @ errors: If dest queue param are equal to src queue param, behavior is undefined.
    @ complexity: O(1).
*/
queue_t *QueueAppend(queue_t *dest, queue_t *src);

#endif /* __QUEUE_H__ */


