/* 
	Developer: Roy Steiner.
	File: Search.
	Date: 28/07/24
	Status: Approved.
*/

#include "search.h"

int RecursiveBinarySearch(int arr[], size_t left, size_t right, int num)
{
	size_t mid = (left + right) >> 1;
	
	if (left > right) 
	{
		return -1;
	}
	
	if (arr[mid] > num)
	{
		mid = RecursiveBinarySearch(arr, left, (mid - 1), num);
	}
	
	else if (arr[mid] < num)
	{
		mid = RecursiveBinarySearch(arr, (mid + 1), right, num);
	}
	
	return (int)mid;
}

int BinarySearch(int arr[], size_t left, size_t right, int num)
{
	size_t mid = 0;
	
	while (left <= right)
	{
		mid = (left + right) >> 1;
		
		if (arr[mid] > num)
		{
			right = (mid - 1);
		}
		
		else if (arr[mid] < num)
		{
			left = (mid + 1);
		}
		
		else
		{ 
			return mid;
		}
	}
	
	return -1;
}


