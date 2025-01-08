#include <stdio.h>
#include <stdint.h> /*uint32_t*/

#include "dhcp.h"

#define UNUSED(x) (void)(x)

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define RESET "\033[0m"

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
	/*--------- Variables declaration ---------*/
	dhcp_ip net_id = {192,168,111,0};
	
	dhcp_ip req_ip = {192,168,111,0};
	dhcp_ip req_ip2 = {192,168,111,255};
	dhcp_ip req_ip3 = {192,168,111,254};
	dhcp_ip alloc_ip;
	/*------------ Create -------------*/
	dhcp_t* dhcp = DHCPCreate(net_id, 24);
		
	/*---------------- Test #1 ----------------*/ 
	fail_message = "Test 1 failed - "BOLD_RED"DHCPLease"RESET;	
	PrintTestResult(fail_message, DHCPLease(dhcp, req_ip, alloc_ip), 7
																	, __LINE__);
	/*---------------- Test #2 ----------------*/ 
	fail_message = "Test 2 failed - "BOLD_RED"DHCPRelease"RESET;	
	PrintTestResult(fail_message
	, DHCPRelease(dhcp, alloc_ip), 0, __LINE__);
	PrintTestResult(fail_message
	, DHCPRelease(dhcp, alloc_ip), 4, __LINE__);

	/*---------------- Test #3 ----------------*/ 
	fail_message = "Test 3 failed - "BOLD_RED"DHCPLease"RESET;	
	PrintTestResult(fail_message
	, DHCPLease(dhcp, req_ip2, alloc_ip), 7, __LINE__);
	PrintTestResult(fail_message
	, DHCPLease(dhcp, req_ip3, alloc_ip), 7, __LINE__);

	/*---------------- Test #4 ----------------*/ 
	fail_message = "Test 4 failed - "BOLD_RED"DHCPCountFree"RESET;	
	PrintTestResult(fail_message
	, DHCPCountFree(dhcp), 251, __LINE__);
	
	/*------------ Destroy ------------*/
	DHCPDestroy(dhcp);
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