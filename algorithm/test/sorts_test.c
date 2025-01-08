#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sorts.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[5;1;32m"
#define RESET "\033[0m"

#define UNUSED(x) (void)(x)

#define SIZE 5000

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

int CompFunc(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

static int IsSort(const int arr[], size_t size)
{
	size_t i = 1;
	
	for (; i < size ; ++i)
	{
		if(arr[i - 1] > arr[i])
		{
			return 0;
		}
	}

	return 1;	
}

static void CreateTest()
{
	char *fail_message = NULL;
	
	size_t i = 0;

	int arr[SIZE] = {0};
	int arr2[SIZE] = {0};
	int arr3[SIZE] = {0};
	int arr4[SIZE] = {0};
	int arr5[SIZE] = {0};
	int arr6[SIZE] = {0};
	int arr7[SIZE] = {0};
	int arr8[SIZE] = {0};
	int arr9[SIZE] = {0};
	
	clock_t start = 0;
	clock_t end = 0;
	
	for (i = 0 ; i < SIZE ; ++i)
	{
		arr[i] =  rand() % 100 + 1;	/* rand 1 - 100 */
		arr2[i] = arr[i];
		arr3[i] = arr[i];
		arr4[i] = arr[i];
		arr5[i] = arr[i];
		arr6[i] = arr[i];
		arr7[i] = arr[i];
		arr8[i] = arr[i];
		arr9[i] = arr[i];
	}
	
	/*---------------------- Test1 ----------------------*/
	start = clock();
	BubbleSort(arr, SIZE);
	end = clock();
	printf("BubbleSort CPU time: %ld\n", (end - start));
	
	fail_message = "Test1 failed - "BOLD_RED"BubbleSort"RESET;
	PrintTestResult(fail_message, IsSort(arr, SIZE), 1, __LINE__); 
	
	/*---------------------- Test2 ----------------------*/
	start = clock();
	SelectionSort(arr2, SIZE);
	end = clock();
	printf("SelectionSort CPU time: %ld\n", (end - start));
	
	fail_message = "Test2 failed - "BOLD_RED"SelectionSort"RESET;
	PrintTestResult(fail_message, IsSort(arr2, SIZE), 1, __LINE__); 
	
	/*---------------------- Test3 ----------------------*/
	start = clock();
	InsertionSort(arr3, SIZE);
	end = clock();
	printf("InsertionSort CPU time: %ld\n", (end - start));
	
	fail_message = "Test3 failed - "BOLD_RED"InsertionSort"RESET;
	PrintTestResult(fail_message, IsSort(arr3, SIZE), 1, __LINE__);
	
	/*---------------------- Test4 ----------------------*/
	start = clock();
	CountingSort(arr4, SIZE);
	end = clock();
	printf("CountingSort CPU time: %ld\n", (end - start));
	
	fail_message = "Test4 failed - "BOLD_RED"CountingSort"RESET;
	PrintTestResult(fail_message, IsSort(arr4, SIZE), 1, __LINE__); 
	
	/*---------------------- Test5 ----------------------*/
	start = clock();
	RadixSort(arr5, SIZE);
	end = clock();
	printf("RadixSort CPU time: %ld\n", (end - start));
	
	fail_message = "Test5 failed - "BOLD_RED"RadixSort"RESET;
	PrintTestResult(fail_message, IsSort(arr5, SIZE), 1, __LINE__);
	
	/*---------------------- Test6 ----------------------*/
	start = clock();
	MergeSort(arr6, SIZE);
	end = clock();
	printf("MergeSort CPU time: %ld\n", (end - start));
	
	fail_message = "Test6 failed - "BOLD_RED"MergeSort"RESET;
	PrintTestResult(fail_message, IsSort(arr6, SIZE), 1, __LINE__);
	
	/*---------------------- Test7 ----------------------*/
	start = clock();
	QSort(arr7, SIZE, sizeof(int), CompFunc);
	end = clock();
	printf("QSort CPU time: %ld\n", (end - start));
	start = clock();
	qsort(arr8, SIZE, sizeof(int), CompFunc);
	end = clock();
	printf("qsort CPU time: %ld\n", (end - start));
	fail_message = "Test7 failed - "BOLD_RED"QSort"RESET;
	PrintTestResult(fail_message, IsSort(arr7, SIZE), 1, __LINE__);

	/*---------------------- Test8 ----------------------*/
	start = clock();
	HeapSort(arr9, SIZE);
	end = clock();
	printf("HeapSort CPU time: %ld\n", (end - start));
	
	fail_message = "Test6 failed - "BOLD_RED"HeapSort"RESET;
	PrintTestResult(fail_message, IsSort(arr9, SIZE), 1, __LINE__);
	
	printf("\n");
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
