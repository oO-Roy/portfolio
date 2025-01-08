#include <stdio.h>		/* printf and size_t */
#include <string.h>		/* strcmp */

#include "dllist.h"	/* forward declaration */

#define UNUSED(x) (void)(x)

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define RESET "\033[0m"

static size_t fail_count = 0;

int IsMatch(const void *data, void *param)
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
	/*--------- Variables declaration ---------*/
	char *fail_message = NULL;
	dllist_t *dllist = NULL;
	dllist_t *dllist2 = NULL;
	
	const int data = 55;
	const int data2 = 77;
	
	dll_iter_t iter = NULL;
	dll_iter_t iter2 = NULL;
	
	/*------------ Create -------------*/
	dllist = DLlistCreate();
	dllist2 = DLlistCreate();
		
	/*---------------- Test #1 ----------------*/ 
	fail_message = "Test 1 failed - "BOLD_RED"DLlistInsertBefore"RESET;	
	iter = DLlistInsertBefore(DLlistEnd(dllist), &data);
	iter2 = DLlistInsertBefore(DLlistEnd(dllist), &data2);
	DLlistInsertBefore(DLlistEnd(dllist), &data2);
	DLlistInsertBefore(DLlistEnd(dllist), &data2);
	DLlistInsertBefore(DLlistEnd(dllist), &data2);
	PrintTestResult(fail_message
	, DLlistIsEmpty((const dllist_t *)dllist), 0, __LINE__);

	/*---------------- Test #2 ----------------*/
	fail_message = "Test 2 failed - "BOLD_RED"DLlistIsSameIter"RESET;
	PrintTestResult(fail_message
	, DLlistIsSameIter(iter, iter), 1, __LINE__);
	PrintTestResult(fail_message
	, DLlistIsSameIter(iter, iter2), 0, __LINE__);
	
	/*---------------- Test #3 ----------------*/
	fail_message = "Test 3 failed - "BOLD_RED"DLlistFindAll"RESET;
	PrintTestResult(fail_message
	, DLlistFindAll(DLlistBegin(dllist), DLlistEnd(dllist)
	, IsMatch,(int *) &data2, dllist2), 1, __LINE__);
	
	/*---------------- Test #4 ----------------*/
	fail_message = "Test 4 failed - "BOLD_RED"DLlistSize"RESET;
	PrintTestResult(fail_message
	, DLlistSize(dllist), 5, __LINE__);
	
	/*---------------- Test #5 ----------------*/
	fail_message = "Test 5 failed - "BOLD_RED"DLlistPushFront"RESET;
	DLlistPushFront(dllist, &data2);
	PrintTestResult(fail_message
	, DLlistSize(dllist), 6, __LINE__);
	
	/*---------------- Test #6 ----------------*/
	fail_message = "Test 6 failed - "BOLD_RED"DLlistPushBack"RESET;
	DLlistPushBack(dllist, &data2);
	PrintTestResult(fail_message
	, DLlistSize(dllist), 7, __LINE__);
	
	/*---------------- Test #7 ----------------*/
	fail_message = "Test 7 failed - "BOLD_RED"DLlistPopFront"RESET;
	DLlistPopFront(dllist);
	PrintTestResult(fail_message
	, DLlistSize(dllist), 6, __LINE__);
	
	/*---------------- Test #8 ----------------*/
	fail_message = "Test 8 failed - "BOLD_RED"DLlistPopBack"RESET;
	DLlistPopBack(dllist);
	PrintTestResult(fail_message
	, DLlistSize(dllist), 5, __LINE__);
	
	/*---------------- Test #9 ----------------*/
	fail_message = "Test 9 failed - "BOLD_RED"DLlistSplice"RESET;
	iter = DLlistEnd(dllist);                                        
	DLlistSplice(DLlistBegin(dllist2), DLlistBegin(dllist), iter);
	PrintTestResult(fail_message, DLlistSize(dllist), 0, __LINE__);
	PrintTestResult(fail_message, DLlistSize(dllist2), 9, __LINE__);
	
	/*------------ Destroy ------------*/
	DLlistDestroy(dllist);
	DLlistDestroy(dllist2);
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


