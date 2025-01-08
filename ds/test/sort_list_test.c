#include <stdio.h>		/* printf and size_t */
#include <string.h>		/* strcmp */

#include "sort_list.h"	/* forward declaration */

#define UNUSED(x) (void)(x)

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define RESET "\033[0m"

static size_t fail_count = 0;

int ActionFunc(void *a, void *b)
{
	*(int *) a *= 2;
	
	UNUSED(b);
	
	return 0;
}

int CompFunc(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

int IsMatch(const void *data, void *param)
{
	return *(int *)data == *(int *)param;
}

void PrintList(sortl_t *sort_list)
{
    sortl_iter_t iter = SortLBegin(sort_list);
    printf("Head -> ");
    while (!SortLIsSameIter(iter, SortLEnd(sort_list)))
    {
        printf("%d -> ", *(int *)SortLGetVal(iter));
        iter = SortLNext(iter);
    }
    printf("NULL\n");
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
	/*--------- Variables declaration ---------*/
	char *fail_message = NULL;
	sortl_t *list = NULL;
	sortl_t *list2 = NULL;
	int data1 = 81;
	int data2 = 22;
	int data3 = 34;
	int data4 = 0;
	sortl_iter_t iter1 = {0};
	sortl_iter_t iter2 = {0};
	
	int param = 2;
	
	
	/*------------ Create -------------*/
	list = SortLCreate(&CompFunc);
	list2 = SortLCreate(&CompFunc);
	PrintTestResult(fail_message
	, SortLIsEmpty(list), 1, __LINE__);
	
	/*---------------- Test #1 ----------------*/
	fail_message = "Test 1 failed - "BOLD_RED"SortLCreate"RESET;
	PrintTestResult(fail_message
	, SortLIsEmpty(list), 1, __LINE__);
	
	/*---------------- Test #2 ----------------*/ 
	fail_message = "Test 2 failed - "BOLD_RED"SortLSize"RESET;	

	SortLInsert(list, &data1);
	SortLInsert(list, &data2);
	SortLInsert(list, &data3);
	PrintTestResult(fail_message
	, SortLSize(list), 3, __LINE__);
	
	/*---------------- Test #3 ----------------*/
	fail_message = "Test 3 failed - "BOLD_RED"SortLFind"RESET;
	iter1 = SortLFind(list , SortLBegin(list), SortLPrev(SortLEnd(list)), &data3);
	iter2 = SortLFind(list , SortLBegin(list), SortLPrev(SortLEnd(list)), &data3);
	PrintTestResult(fail_message, *(int*)SortLGetVal(iter1)
										, *(int*)SortLGetVal(iter2) , __LINE__);
	
	/*---------------- Test #4 ----------------*/
	fail_message = "Test 4 failed - "BOLD_RED"SortLForEach"RESET;
	PrintList(list);
	SortLForEach(SortLBegin(list), SortLEnd(list), ActionFunc,  &param);
	PrintList(list);
	printf("%d\n", *(int*)SortLGetVal(SortLFind(list , SortLBegin(list), SortLPrev(SortLEnd(list)), &data4)));
	 	 
	/*---------------- Test #5 ----------------*/
	fail_message = "Test 5 failed - "BOLD_RED"SortLMerge"RESET; 	 
	SortLInsert(list2, &data1);
	SortLInsert(list2, &data2);
	SortLInsert(list2, &data3); 
	puts("list1 before SortLMerge");
    PrintList(list);
    puts("list2 before SortLMerge");
    PrintList(list2);
    SortLMerge(list, list2);
    puts("\nlist1 after SortLMerge");
    PrintList(list);
    puts("list2 after SortLMerge");
    PrintList(list2);
    
    /*---------------- Test #6 ----------------*/
	fail_message = "Test 6 failed - "BOLD_RED"SortLFind"RESET; 	
	puts("\nlist1 before SortLRemove"); 
	PrintList(list);	 
	SortLRemove(SortLBegin(list));
	puts("\nlist1 after SortLRemove"); 
	PrintList(list);
	 	 
	/*------------ Destroy ------------*/
	SortLDestroy(list);
	SortLDestroy(list2);

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
