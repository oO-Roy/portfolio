#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    MemoryMappingCode(1,1.0f,1.0);
		
	printf("\n\nenvp address: %p\n", (void *)envp);	
	printf("argv address: %p\n", (void *)argv);
   
    return 0;
}
