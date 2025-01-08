#include <stdio.h>

#include "sllist.h"

#define UNUSED(x) (void)(x)

int main()
{
	int x = 5, y = 7;
	
    sllist_t *list = NULL;
    iterator_t iter = NULL;

    list = SLLCreate();
    iter = SLLBegin(list); 
    SLLInsertBefore(iter, &x);
    printf("The Number in iter: %d\n" ,(*(int *)SLLGetData(iter)));
    SLLInsertBefore(iter, &y);
    printf("The Number in iter: %d\n" ,(*(int *)SLLGetData(iter)));
    printf("The amount of elements in the list: %lu\n" ,SLLCount(list));
    SLLDestroy(list);


    return 0;
}
