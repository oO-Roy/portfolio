#include <stdio.h>

#include "queue.h"

int main()
{
	const int a = 1, b = 2, c = 3, d = 4;
	
	queue_t *queue = NULL, *queue2 = NULL;
	 
	queue = QueueCreate();	
	queue2 = QueueCreate();
		
	QueueEnqueue(queue, &a);	
	QueueEnqueue(queue, &b);
	QueueEnqueue(queue2, &c);	
	QueueEnqueue(queue2, &d);
	
	printf("queue size: %lu\n", QueueSize(queue));
	printf("queue2 size: %lu\n", QueueSize(queue2));
	
	printf("queue peek : %d\n", *(int *)QueuePeek(queue));
	printf("queue2 peek : %d\n\n", *(int *)QueuePeek(queue2));
	

	printf("appending queue2 to queue1...\n\n");
	QueueAppend(queue, queue2);
	
	printf("queue2 is empty? : %d\n\n", QueueIsEmpty(queue2));
	
	printf("queue peek : %d\n", *(int *)QueuePeek(queue));
	printf("Dequeuing 1st element..\n\n");
	QueueDequeue(queue);
	
	printf("queue peek : %d\n", *(int *)QueuePeek(queue));
	printf("Dequeuing 2nd element..\n\n");
	QueueDequeue(queue);

	printf("queue peek : %d\n", *(int *)QueuePeek(queue));
	printf("Dequeuing 3rd element..\n\n");
	QueueDequeue(queue);
	
	printf("queue peek : %d\n", *(int *)QueuePeek(queue));
	printf("Dequeuing 4th element..\n\n");
	QueueDequeue(queue);
	
	printf("queue is empty? : %d\n", QueueIsEmpty(queue));
	
	QueueDestroy(queue);	
	QueueDestroy(queue2);	
	
	return 0;
}
