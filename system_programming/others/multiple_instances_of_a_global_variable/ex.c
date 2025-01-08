#include <stdio.h>

int x = 10;

void PrintGlobalVariableAddress(void)
{
	printf("variable x address: %p\n", &x);
}
