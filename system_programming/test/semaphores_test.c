#include <stdio.h>
#include "semaphores.h"

int main(int argc, char *argv[])
{
	if (2 > argc)
	{
		printf("Error!\nNo semaphore name\n");
		return 1;
	}
	
	sem_manipulation(argv[1]);

	return 0;
}
