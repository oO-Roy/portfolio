#include <stdio.h>

#include "stack.h"

int main()
{
	stack_ptr_t stacky;
	
	int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8;
	int *a_p = 0, *b_p = 0, *c_p = 0, *d_p = 0, *e_p = 0, *f_p = 0, *g_p = 0, *h_p = 0;

	stacky = StackCreate(4, 4);
	printf("\nCreating new stack...\n\n");
	printf("Is empty: %d\n", StackIsEmpty(stacky));
	printf("Stack capacity: %ld\n\n", StackCapacity(stacky));
	printf("The stack is empty, starting to push four elements...\n\n");
	
	StackPush(stacky, &a);
	StackPush(stacky, &b);
	StackPush(stacky, &c);
	StackPush(stacky, &d);
	
	printf("Peeking: %d\n", *(int *)StackPeek(stacky));
	printf("Stack size: %lu\n\n", StackSize(stacky));
	
	d_p = StackPop(stacky);
	c_p = StackPop(stacky);
	b_p = StackPop(stacky);
	a_p = StackPop(stacky);
	
	printf("Pop #1: %d\n", *d_p);
	printf("Pop #2: %d\n", *c_p);
	printf("Pop #3: %d\n", *b_p);
	printf("Pop #4: %d\n\n", *a_p);
	
	printf("Is empty: %d\n", StackIsEmpty(stacky));
	printf("Stack size: %lu\n\n", StackSize(stacky));
	
	printf("The stack is empty, pushing another four elements...\n\n");
	
	StackPush(stacky, &e);
	StackPush(stacky, &f);
	StackPush(stacky, &g);
	StackPush(stacky, &h);
	
	printf("Peeking: %d\n", *(int *)StackPeek(stacky));
	printf("Stack size: %lu\n\n", StackSize(stacky));
	
	h_p = StackPop(stacky);
	g_p = StackPop(stacky);
	f_p = StackPop(stacky);
	e_p = StackPop(stacky);
	
	printf("Pop #5: %d\n", *h_p);
	printf("Pop #6: %d\n", *g_p);
	printf("Pop #7: %d\n", *f_p);
	printf("Pop #8: %d\n\n", *e_p);
	
	printf("Is empty: %d\n", StackIsEmpty(stacky));
	printf("Stack size: %lu\n\n", StackSize(stacky));
	
	printf("The stack is empty again, destroying stack...\n\n");
	
	StackDestroy(stacky);
	
	return 0;
}
