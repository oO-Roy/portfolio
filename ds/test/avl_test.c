#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[5;1;32m"
#define RESET "\033[0m"

#define UNUSED(x) (void)(x)

static int CompFunc(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

static int ActionPrint(void *data , void *param)
{
	UNUSED(param);
	
	printf("%d\n", *(int *)data); 
	
	return 0;
}

static int ActionFunc(void *data , void *param)
{
	UNUSED(param);
	
	*(int *)data += 1; 
	
	return 0;
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
	char *fail_message = NULL;
	
	/*---------------- Test variables decleration ----------------*/
	avl_t *tree = AVLCreate(CompFunc);
	int data1 = 100;
	int data2 = 200;
	int data3 = 50;
	int data4 = 170;
	int data5 = 30;
	int data6 = 40;

	int dummy_data = 1000000;
	/*---------------- Test #1 ----------------*/
	fail_message = "Test 1 failed - "BOLD_RED"BSTCreate"RESET;
	PrintTestResult(fail_message, AVLIsEmpty(tree), 1, __LINE__);
	
	/*---------------- Test #2 ----------------*/
	fail_message = "Test 2 failed - "BOLD_RED"AVLHeight"RESET;
	PrintTestResult(fail_message, AVLHeight(tree), 0, __LINE__);

	/*---------------- Test #3 ----------------*/
	fail_message = "Test 3 failed - "BOLD_RED"AVLInsert"RESET;
	AVLInsert(tree, &data1);
	PrintTestResult(fail_message, AVLSize(tree), 1, __LINE__);
	AVLInsert(tree, &data2);
	AVLInsert(tree, &data3);
	AVLInsert(tree, &data4);
	AVLInsert(tree, &data5);
	AVLInsert(tree, &data6);

	printf("root height: %ld\n", AVLHeight(tree));

	AVLForEach(tree, PRE_ORDER, ActionPrint, &dummy_data);
	printf("\n");

	PrintTestResult(fail_message, AVLSize(tree), 6, __LINE__);

	/*---------------- Test #4 ----------------*/
	fail_message = "Test 4 failed - "BOLD_RED"AVLFind"RESET;
	PrintTestResult(fail_message, *(int *)AVLFind(tree, &data1)
												, data1, __LINE__);
	PrintTestResult(fail_message, *(int *)AVLFind(tree, &data2)
												, data2, __LINE__);
	PrintTestResult(fail_message, *(int *)AVLFind(tree, &data3)
												, data3, __LINE__);
	PrintTestResult(fail_message, *(int *)AVLFind(tree, &data4)
												, data4, __LINE__);

	/*---------------- Test #5 ----------------*/
	fail_message = "Test 5 failed - "BOLD_RED"AVLRemove"RESET;
	PrintTestResult(fail_message
				, *(int *)AVLRemove(tree, &data1), 100, __LINE__);
				
	AVLForEach(tree, PRE_ORDER, ActionPrint, &dummy_data);
	printf("\n");
	
	AVLForEach(tree, IN_ORDER, ActionPrint, &dummy_data);
	printf("\n");
	
	AVLForEach(tree, IN_ORDER, ActionPrint, &dummy_data);
	printf("\n");
	
	PrintTestResult(fail_message
				, *(int *)AVLRemove(tree, &data2), 200, __LINE__);

	AVLForEach(tree, IN_ORDER, ActionPrint, &dummy_data);
	printf("\n");

	/*---------------- Test #6 ----------------*/
	fail_message = "Test 6 failed - "BOLD_RED"AVLForEach(preorder)"RESET;
	AVLForEach(tree, PRE_ORDER, ActionFunc, &dummy_data);


	AVLForEach(tree, PRE_ORDER, ActionPrint, &dummy_data);
	printf("\n");
	AVLForEach(tree, IN_ORDER, ActionPrint, &dummy_data);
	printf("\n");

	AVLInsert(tree, &data1);
	PrintTestResult(fail_message,  *(int *)AVLRemove(tree, &data4)
											, 171, __LINE__);

	AVLForEach(tree, IN_ORDER, ActionPrint, &dummy_data);
	printf("\n");

	PrintTestResult(fail_message,  *(int *)AVLRemove(tree, &data6)
											, 41, __LINE__);

	AVLForEach(tree, IN_ORDER, ActionPrint, &dummy_data);
	printf("\n");

	PrintTestResult(fail_message,  *(int *)AVLRemove(tree, &data3)
															, 51, __LINE__);

	AVLForEach(tree, IN_ORDER, ActionPrint, &dummy_data);
	printf("\n");		

	PrintTestResult(fail_message,  *(int *)AVLRemove(tree, &data1)
															, 100, __LINE__);

	AVLForEach(tree, IN_ORDER, ActionPrint, &dummy_data);
	printf("\n");		
	/*---------------- Destroy ----------------*/
	AVLDestroy(tree);
	
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
