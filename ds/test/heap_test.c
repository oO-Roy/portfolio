#include <stdio.h>
#include <string.h> /*strcmp & strlen*/

#include "heap.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[5;1;32m"
#define RESET "\033[0m"

static size_t fail_count = 0;

int CompFunc(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

int IsMatch(const void *data, const void *param)
{
	return *(int *)data == *(int *)param;
}

static void PrintTestResult(const char *fail_message
										, size_t actual
										, size_t expected
										, size_t line_num)
{
	if(actual != expected)
	{
		printf("Test " BOLD_RED"Failed "RESET
		 "on (" BOLD_RED "line %lu" RESET ")\nfailure message: %s\n\n"
		 										, line_num, fail_message);
		++fail_count;
	}
}

static void CreateTest()
{
	char *fail_message = NULL;
	
	/*---------------- Test variables decleration ----------------*/
	heap_t *heap = HeapCreate(&CompFunc);
	int num1 = 8;
	int num2 = 5;
	int num3 = 3;
	int num4 = 1;
	int num5 = 9;

	int peek = 0;
	int pop = 0;
	int removed_data = 0;

	/*---------------- Test #1 ----------------*/
	fail_message = "Test 1 failed - "BOLD_RED"HeapCreate"RESET;
	PrintTestResult(fail_message, HeapIsEmpty(heap), 1, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 0, __LINE__);

	/*---------------- Test #2 ----------------*/
	fail_message = "Test 2 failed - "BOLD_RED"HeapPush"RESET;
	PrintTestResult(fail_message, HeapPush(heap, &num1), 0, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 1, __LINE__);

	/*---------------- Test #3 ----------------*/
	fail_message = "Test 3 failed - "BOLD_RED"HeapPeek"RESET;
	peek = *(int*)HeapPeek(heap);
	PrintTestResult(fail_message, peek, 8, __LINE__);

	PrintTestResult(fail_message, HeapPush(heap, &num2), 0, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 2, __LINE__);
	peek = *(int*)HeapPeek(heap);
	PrintTestResult(fail_message, peek, 5, __LINE__);

	PrintTestResult(fail_message, HeapPush(heap, &num3), 0, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 3, __LINE__);
	peek = *(int*)HeapPeek(heap);
	PrintTestResult(fail_message, peek, 3, __LINE__);

	PrintTestResult(fail_message, HeapPush(heap, &num4), 0, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 4, __LINE__);
	peek = *(int*)HeapPeek(heap);
	PrintTestResult(fail_message, peek, 1, __LINE__);

	PrintTestResult(fail_message, HeapPush(heap, &num5), 0, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 5, __LINE__);
	peek = *(int*)HeapPeek(heap);
	PrintTestResult(fail_message, peek, 1, __LINE__);

	/*---------------- Test #4 ----------------*/
	fail_message = "Test 4 failed - "BOLD_RED"HeapPop"RESET;
	pop = *(int*)HeapPop(heap);
	PrintTestResult(fail_message, pop, num4, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 4, __LINE__);

	pop = *(int*)HeapPop(heap);
	PrintTestResult(fail_message, pop, num3, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 3, __LINE__);
	
	/*---------------- Test #5 ----------------*/
	fail_message = "Test 5 failed - "BOLD_RED"HeapPop"RESET;
	removed_data = *(int*)HeapRemove(heap, IsMatch, &num5);
	PrintTestResult(fail_message, removed_data, num5, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 2, __LINE__);

	removed_data = *(int*)HeapRemove(heap, IsMatch, &num2);
	PrintTestResult(fail_message, removed_data, num2, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 1, __LINE__);

	removed_data = *(int*)HeapRemove(heap, IsMatch, &num1);
	PrintTestResult(fail_message, removed_data, num1, __LINE__);
	PrintTestResult(fail_message, HeapSize(heap), 0, __LINE__);

	/*---------------- Destroy ----------------*/
	HeapDestroy(heap);
}

int main()
{
	CreateTest();													

	if(0 != fail_count)
	{
		printf("Total tests "BOLD_RED"failed "RESET": %lu\n", fail_count);
	}
	else
	{
		printf(BOLD_GREEN "\n===================================\n");
		printf(BOLD_GREEN "||   ^ ^                         ||\n"); 
		printf(BOLD_GREEN "||  (0,0)    ALL TESTS PASSED!   ||\n");
		printf(BOLD_GREEN "||  (   )                        ||\n");
		printf(BOLD_GREEN "||  -\"-\"-------------------------||\n" RESET);
		printf(BOLD_GREEN "===================================\n\n" RESET);
	}
	return 0;
}
