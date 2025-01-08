#include <stdio.h>

#include "pq_by_heap.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define RESET "\033[0m"

int CompFunc(const void *a, const void *b)
{
	return (*(int *)b - *(int *)a);
}

int IsMatch(const void *data, const void *param)
{
	return *(int *)data == *(int *)param;
}

static size_t fail_count = 0;

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
	/*---------------- Test #Decleration ----------------*/
	char *fail_message = NULL;
	pq_t *pq  = PQCreate(&CompFunc);
	int num = 100;
	int num2 = 200;
	int num3 = 300;
	
	/*---------------- Test #1 ----------------*/
	fail_message = "Test 1 failed - "BOLD_RED"PQCreate"RESET;
	PrintTestResult(fail_message
	, PQIsEmpty(pq), 1, __LINE__);
	
	/*---------------- Test #2 ----------------*/
	fail_message = "Test 2 failed - "BOLD_RED"PQEnqueue"RESET;	
	PQEnqueue(pq, (void *) &num);
	PQEnqueue(pq, (void *) &num2);
	PQEnqueue(pq, (void *) &num3);
	PrintTestResult(fail_message, PQSize(pq), 3, __LINE__); 
	
	/*---------------- Test #3 ----------------*/
	fail_message = "Test 3 failed - "BOLD_RED"PQDequeue"RESET;
	PQDequeue(pq);
	PrintTestResult(fail_message, PQSize(pq), 2, __LINE__);
	
	/*---------------- Test #4 ----------------*/
	fail_message = "Test 4 failed - "BOLD_RED"PQPeek"RESET;
	PrintTestResult(fail_message
	, *(int *)PQPeek(pq), 200, __LINE__);
	
	/*---------------- Test #5 ----------------*/
	fail_message = "Test 5 failed - "BOLD_RED"PQErase"RESET;
	PrintTestResult(fail_message
	, *(int *)PQErase(pq, IsMatch, &num2), 200, __LINE__);
	
	/*---------------- Test #6 ----------------*/
	fail_message = "Test 6 failed - "BOLD_RED"PQClear"RESET;
	PQClear(pq);
	PrintTestResult(fail_message, PQIsEmpty(pq), 1, __LINE__);
	
	/*---------------- Destroy ----------------*/
	PQDestroy(pq);
	
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
