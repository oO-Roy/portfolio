#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#include "memory_mapping.h"

#define BUFFER_SIZE 100

typedef struct range
{
	size_t from;
	size_t to;
} range_t;


static range_t ParseRangeFromLine(const char *line)
{
	char left_overs[BUFFER_SIZE];
	range_t res = { 0, 0};
	
	sscanf(line, "%lx-%lx%s", &res.from, &res.to, left_overs);
	
	return res;
}

typedef int (*func_ptr) (int ,float ,double);

static void PrintSegmentOfRange(range_t range)
{
	size_t i;
	static int bss_var;
	static int data_var = 1;
	int stack_var = 1;
	int *heap_var = (int *) malloc(sizeof(int));
	func_ptr text_var = MemoryMappingCode;
	
	const char *segments[5] = { ".BSS", ".DATA", ".STACK", ".HEAP", ".TEXT" }; 

	size_t addresses[5];
	addresses[0] = (size_t ) &bss_var;
	addresses[1] = (size_t ) &data_var;
	addresses[2] = (size_t ) &stack_var;
	addresses[3] = (size_t ) heap_var;
	addresses[4] = (size_t ) text_var;
	
	for (i = 0; i < 5; ++i)
	{
		if (range.from <= addresses[i] && range.to > addresses[i])
		{
			printf("%s range is [%lx, %lx)\n", segments[i], range.from, range.to);
		}
	}
}

static void PrintSegmentAddresses()
{
	range_t range;
	FILE *fp = fopen("/proc/self/maps", "r");
	char buffer[BUFFER_SIZE];
	if (NULL == fp)
	{
		printf("failed to open /proc/self/maps file\n");
		
		return;
	}
	
	printf("\n");
	while (NULL != fgets(buffer, BUFFER_SIZE, fp))
	{
		range = ParseRangeFromLine(buffer);
		PrintSegmentOfRange(range);
	}
	
	fclose(fp);
}






const int global_var1 = 1;
int global_var2 = 2;
static const int global_var3 = 3;
static int global_var4 = 4;

static void StaticFunction()
{
	
}

void ExternFunction()
{
	
}

int MemoryMappingCode(int functions_arg_1,float functions_arg_2,double functions_arg_3)
{
    int* p_heap_var = (int*)malloc(sizeof(int));
    
    const int const_local = 5;
    
    int non_const_local = 5;
    
    static const int static_local_var1 = 111;
    static int static_local_var2 = 222;
    
    char* string_literal = "abc";
    const char *string_liteal2 = "bcd";

	printf("extern function address: %p\n", &ExternFunction);
	printf("static function address: %p\n", &StaticFunction);
	printf("heap variable p_heap_var address: %p\n", p_heap_var);
	printf("Local const variable const_local address: %p\n", &const_local);
	printf("Local non-const variable non_const_local address: %p\n", &non_const_local);
	printf("Global const variable global_var1 address: %p\n", &global_var1);
	printf("Global non-const variable global_var2 address: %p\n", &global_var2);
	printf("Local Static const variable static_local_var1 address: %p\n", &static_local_var1);
	printf("Local Static non-const variable static_local_var2 address: %p\n", &static_local_var2);
	printf("Global Static const variable global_var3 address: %p\n", &global_var3);
	printf("Global Static non-const variable global_var4 address: %p\n", &global_var4);
	printf("const string literal string_liteal2 address: %p\n", string_liteal2);
	printf("non-const string literal string_literal address: %p\n", string_literal);
	PrintSegmentAddresses();
    return 0;
}

