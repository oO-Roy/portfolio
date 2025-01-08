#include<stdio.h>
#include<dlfcn.h>

typedef int (foo_func) (int num);
void *lib_handle = NULL;
foo_func *foo;

int main()
{
	lib_handle = (void *)dlopen("./libex.so", RTLD_LAZY);
	if(lib_handle) 
	{
		printf("dlopen returns %p\n", lib_handle);
		*(void **) &foo = dlsym(lib_handle, "Foo");
		
		if(foo)
		{
			printf("dlsym returns foo = %p\n", foo);
		  	foo(999);
		}
	
		else
		{
			printf("Function entry not found in DLL");
		}
		
		dlclose(lib_handle);
	}
	
	else
	{
		printf("Unable to open DLL");
	}
	
	return 0;
}
