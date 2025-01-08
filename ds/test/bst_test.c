#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

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

static int ActionFunc(void *data , void *param)
{
	UNUSED(param);
	
	*(int *)data *= 2; 
	
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

/*static void PrintTreeAsLinkedList(bst_t *bst)
{
	bst_iter_t iter = BSTBegin(bst);

    while(!BSTIsSameIter(iter, BSTEnd(bst)))
    {
        printf("%d \n", *(int *) BSTGetData(iter));
        iter = BSTNext(iter);
    }
    
     printf("\n");
}
*/
static void CreateTest()
{
	char *fail_message = NULL;
	
	/*---------------- Test variables decleration ----------------*/
	bst_t *bst = BSTCreate(CompFunc);
	bst_iter_t iter = NULL;
	bst_iter_t iter2 = NULL;
	bst_iter_t iter3 = NULL;
	bst_iter_t iter4 = NULL;
	bst_iter_t iter5 = NULL;
	
	
	const int data1 = 100;
	const int data2 = 200;
	const int data3 = 50;
	const int data4 = 170;
	const int data5 = 250;
	const int data6 = 75;
	const int data7 = 80;
	const int data8 = 20;
	const int data9 = 15;
	const int data10 = 25;
	
	const int data100 = 555;
	
	/*---------------- Test #1 ----------------*/
	fail_message = "Test 1 failed - "BOLD_RED"BSTCreate"RESET;
	PrintTestResult(fail_message, BSTIsEmpty(bst), 1, __LINE__); 
	
	/*---------------- Test #2 ----------------*/
	fail_message = "Test 2 failed - "BOLD_RED"BSTInsert"RESET;
	
	iter2 = BSTInsert(bst, &data1);
	iter5 = BSTInsert(bst, &data2);
	BSTInsert(bst, &data3);
	iter = BSTInsert(bst, &data4);
	BSTInsert(bst, &data5);
	iter4 = BSTInsert(bst, &data6);
	BSTInsert(bst, &data7);
	iter3 = BSTInsert(bst, &data8);
	BSTInsert(bst, &data9);
	BSTInsert(bst, &data10);
	
	PrintTestResult(fail_message, BSTSize(bst), 10, __LINE__);

	/*---------------- Test #3 ----------------*/
	fail_message = "Test 3 failed - "BOLD_RED"BSTRemove"RESET;
	
	BSTRemove(iter);
	BSTRemove(iter2);
	BSTRemove(iter3);
	BSTRemove(iter4);
	BSTRemove(iter5);
	
	PrintTestResult(fail_message, BSTSize(bst), 5, __LINE__);
		
	/*---------------- Test #4 ----------------*/
	fail_message = "Test 4 failed - "BOLD_RED"BSTFind"RESET;
	PrintTestResult(fail_message
	, *(int *)BSTGetData(BSTFind(bst, &data3)), 50, __LINE__); 
	PrintTestResult(fail_message
	, *(int *)BSTGetData(BSTFind(bst, &data7)), 80, __LINE__);
	PrintTestResult(fail_message
	, *(int *)BSTGetData(BSTFind(bst, &data5)), 250, __LINE__);
	PrintTestResult(fail_message
	, *(int *)BSTFind(bst, &data100), *(int *)BSTEnd(bst), __LINE__);
	
	/*---------------- Test #5 ----------------*/
	fail_message = "Test 5 failed - "BOLD_RED"BSTForEach"RESET;
	PrintTestResult(fail_message
	, BSTForEach(BSTBegin(bst), BSTEnd(bst), ActionFunc, NULL), 0, __LINE__); 
	
	/*---------------- Destroy ----------------*/
	BSTDestroy(bst);
	
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
	printf(BOLD_GREEN"⠀⠀⠀⠀⠀⠀⠀⠀⢀⣄⣀⣀⣠⡴⠶⣄⠀⢀⣤⣤⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣄⠀⠹⠤⠃⠀⡏⠉⠉⠳⢾⡿⣻⡆⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢦⡀⠀⠀⠀⠇⠀⠀⠀⣠⠞⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣴⡀⡀⠀⣴⣆⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣷⣷⣷⣶⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢨⣿⡿⢻⣿⢻⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠏⠜⠀⠈⢉⣿⡟⢿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡞⠁⠀⠀⠀⠀⠀⠉⠁⠈⢻⠳⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⢀⡴⠋⠀⠀⠀⠀⢀⣤⣄⠀⠀⠀⠀⠣⠙⢦⡀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⣰⠟⠀⠀⠀⠀⠀⠀⣿⣿⣿⠀⠀⠀⠀⠀⠰⠂⠙⣦⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⢀⡾⠁⠀⠀⠀⠀⣀⣴⣾⣿⣿⣿⣿⣶⣤⡀⠀⠀⠐⣂⠈⢳⡄⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⣠⠟⠀⠀⠀⠀⠀⣴⣿⣿⡿⣿⣿⣿⠿⣿⣿⣿⣆⠀⠀⠰⢒⠵⢻⣆⠀⠀⠀\n");
    printf("⠀⠀⣰⠏⠀⠀⠀⠀⠀⠀⣿⣿⣿⠀⣿⣿⣿⠀⠈⢿⣿⣿⠄⠀⠀⠄⢊⡡⠜⣦⠀⠀\n");
    printf("⠀⢠⡏⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣷⣿⣿⣿⠀⠀⠈⠙⠉⠀⠀⠀⢒⡡⠔⣋⠼⡇⠀\n");
    printf("⠀⣼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⣷⣶⣄⠀⠀⠀⠀⠀⠐⣈⠤⠒⢻⡄\n");
    printf("⢀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣟⠛⢿⣿⣿⣷⠀⠀⠀⠐⣉⠤⠒⣉⠬⣇\n");
    printf("⢸⠇⢀⠀⠀⠀⠀⠀⠀⣠⣤⡀⠀⠀⣿⣿⣿⠀⠀⣹⣿⣿⡇⠀⠀⡈⠤⠒⣉⠤⠀⣿\n");
    printf("⢸⠀⠘⣆⡀⠀⠀⠀⠀⢿⣿⣿⣄⠀⣿⣿⣿⢀⣴⣿⣿⡿⠀⠀⠀⠤⠒⣉⠤⠒⠀⡿\n");
    printf("⢸⡆⢰⡆⠁⠀⠀⠀⠀⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠀⠀⠀⠠⢒⣉⠤⠒⠁⣸⠃\n");
    printf("⠀⢳⡀⠙⠒⢷⣀⠀⠀⠀⠀⠈⠛⠻⣿⣿⣿⠛⠉⠀⠀⠀⠀⠐⢊⣁⠤⠒⠋⣠⠏⠀\n");
    printf("⠀⠀⠳⣤⣧⡀⠸⡀⠀⠀⠀⠀⠀⠀⠻⣿⠟⠀⠀⠀⠀⠀⠔⣊⡡⠤⠒⢉⡴⠋⠀⠀\n");
    printf("⠀⠀⠀⠀⠙⠳⠦⣌⣁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣁⣤⣴⡾⠟⠋⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠛⠒⠒⠶⠦⠤⠤⠤⠴⠶⠒⠒⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀\n"RESET);
	}
	return 0;
}
