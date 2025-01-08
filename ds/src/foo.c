#include <stdio.h>
#include <assert.h>

#include "foo.h"

void foo(int num)
{
	assert(0 < num);	
	
	printf("Your number is: %d\n", num);
}
