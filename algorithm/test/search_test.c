#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "search.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[5;1;32m"
#define RESET "\033[0m"

#define UNUSED(x) (void)(x)

#define SIZE 900000
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
	
	int i = 0;
	
	int arr[] = {0,1,2,3,4,5,6,7,8,9};
	int arr2[SIZE] = {0};
	int arr3[SIZE] = {0};
	
	clock_t start = 0;
	clock_t end = 0;
	
	for (i = 0 ; i < SIZE ; ++i)
	{
		arr2[i] =  rand() % 100;	/* rand 1 - 100 */
		arr3[i] = arr2[i];
	}
	
	arr2[SIZE - 1] = 777;
	arr3[SIZE - 1] = 777;
	
	/*---------------------- Test1 ----------------------*/
	start = clock();
	BinarySearch(arr2, 0, SIZE - 1, 777);
	end = clock();
	printf("BinarySearch CPU time: %ld\n", (end - start));
	
	fail_message = "Test1 failed - "BOLD_RED"BinarySearch"RESET;
	PrintTestResult(fail_message, BinarySearch(arr, 0, 9, 8), 8, __LINE__); 

	/*---------------------- Test2 ----------------------*/
		start = clock();
	RecursiveBinarySearch(arr3, 0, SIZE - 1, 777);
	end = clock();
	printf("RecursiveBinarySearch CPU time: %ld\n", (end - start));
	
	fail_message = "Test2 failed - "BOLD_RED"RecursiveBinarySearch"RESET;
	PrintTestResult(fail_message, RecursiveBinarySearch(arr, 0, 9, 8), 8, __LINE__);
	
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
