#include <stdio.h>
#include "vector.h"

#define UNUSED(x) ((void)x)

int main()
{
	int a = 1, b = 2, c = 3;
	
	vector_ptr_t vec1_ptr;

	vec1_ptr = VectorCreate(sizeof(int), 3);

	VectorAppend(vec1_ptr, &c);
	
	VectorAppend(vec1_ptr, &b);
	
	VectorAssign(vec1_ptr, 1, &a);
	VectorAssign(vec1_ptr, 2, &b);
	
	
	VectorPop(vec1_ptr, &c);
	
	VectorDestroy(vec1_ptr);
	
	return 0;
}
