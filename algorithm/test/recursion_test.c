#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "recursion.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[5;1;32m"
#define RESET "\033[0m"

#define UNUSED(x) (void)(x)

static size_t fail_count = 0;

int CompFunc(const void *a, const void *b)
{
	return (*(int *)a > *(int *)b);
}

static node_t *BuildList()
{
	node_t *node1 = (node_t *)malloc(sizeof(node_t));
	node_t *node2 = (node_t *)malloc(sizeof(node_t)); 
	node_t *node3 = (node_t *)malloc(sizeof(node_t));
	node_t *node4 = (node_t *)malloc(sizeof(node_t));
	
	node1->data = 1;
	node2->data = 2;
	node3->data = 3;
	node4->data = 4;
	
	node1->next = node2;
	node2->next = node3;
	node3->next = node4;
	node4->next = NULL;
	
	return node1;
}

static void FreeList(node_t *list)
{
	free(list->next->next->next);
	free(list->next->next);
	free(list->next);
	free(list); list = NULL;
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
	/*---------------------- Variables decleration ----------------------*/
	char *fail_message = NULL;
	
	node_t *head = BuildList();
	stack_ptr_t stack = StackCreate(sizeof(int), 4);
	
	int a = 1, b = 2, c = 3, d = 4;
	
	char *str = "Roy";
	char str2[15] = {'0'};
	char *str3 = "Roz";
	char *str4 = "DESTRoy!!!";
	char *str5 = "";
	
	
	/*---------------------- Test Fibonacci ----------------------*/
	
	fail_message = "Test Fibonacci failed - "BOLD_RED"Fibonacci"RESET;
	PrintTestResult(fail_message, Fibonacci(6), 8, __LINE__); 
	
	/*---------------------- Test FlipList ----------------------*/
	
	puts("List BEFORE Flip:");
	printf("head -> %d -> %d -> %d -> %d -> NULL\n", head->data,
	head->next->data, head->next->next->data, head->next->next->next->data);
	
	head = FlipList(head);
	
	puts("\nList AFTER Flip:");
	printf("head -> %d -> %d -> %d -> %d -> NULL\n", head->data,
	head->next->data, head->next->next->data, head->next->next->next->data);
    
    FreeList(head);
    /*---------------------- Test SortStack ----------------------*/
    
	StackPush(stack, &b);
	StackPush(stack, &a);
	StackPush(stack, &d);
	StackPush(stack, &c);
	
	SortStack(stack, &CompFunc);
	
	printf("\nPop #1: %d\n", *(int *)StackPop(stack));
	printf("Pop #2: %d\n", *(int *)StackPop(stack));
	printf("Pop #3: %d\n", *(int *)StackPop(stack));
	printf("Pop #4: %d\n\n", *(int *)StackPop(stack));
	
	StackDestroy(stack);
	/*---------------------- Test strings 1 ----------------------*/
  
   	printf("Recursive str length: %ld\n", RecursiveStrLen(str));
   	printf("Recursive StrCpy: %s\n", RecursiveStrCpy(str2, str));
   	printf("Recursive StrCmp: %d\n", RecursiveStrCmp(str, str3));
   	printf("Recursive StrCat: %s\n", RecursiveStrCat(str2, str3));
   	printf("Recursive StrStr found: %s\n", RecursiveStrStr(str4, str));
	printf("Recursive StrStr needle size is 0: %s\n", RecursiveStrStr(str4, str5));
	printf("Recursive StrStr not found: %s\n", RecursiveStrStr(str4, str3));		

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
	printf(BOLD_GREEN"⠀⠀⠀⠀⠀⠀⠀⠀⢀⣄⣀⣀⣠⡴⠶⣄⠀⢀⣤⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
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
