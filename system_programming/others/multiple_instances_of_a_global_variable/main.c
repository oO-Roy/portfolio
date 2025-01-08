#include <stdio.h>
#include <dlfcn.h>

typedef void (foo_func) (void);

void *lib_handle = NULL;
void *lib_handle2 = NULL;

foo_func *foo;
foo_func *foo2;

int main()
{
	/*printf("Address: %p\n", &x);
	printf("Value: %d\n", x);
	*/PrintGlobalVariableAddress();
	
	lib_handle = (void *)dlopen("./lib2.so", RTLD_LAZY);
	if(lib_handle) 
	{
		foo = dlsym(lib_handle, "Foo");
		
		if(foo)
		{
		  	foo();
		}
		
		dlclose(lib_handle);
	}
	
	lib_handle2 = (void *)dlopen("./lib3.so", RTLD_LAZY);
	if(lib_handle2) 
	{
		foo2 = dlsym(lib_handle2, "Foo2");
		
		if(foo2)
		{
		  	foo2();
		}
		
		dlclose(lib_handle2);	
	}
	
	return 0;
}
