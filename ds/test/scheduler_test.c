#include <stdio.h>

#include "scheduler.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[5;1;32m"
#define RESET "\033[0m"

#define UNUSED(x) (void)(x)

ilrd_uid_t test7_uid;

typedef struct rem
{
	sched_t *sch;
	ilrd_uid_t u_id;
}rem_t;

int FinishActionFunc(void *data)
{
	UNUSED(data);
	
	printf("Finish action function is at process\n");
	return 0;
}

int UnfinishActionFunc(void *data)
{
	UNUSED(data);
	
	printf("Unfinish action function is at process\n");
	return 1;
}

void CleanFunc(void *data)
{
	UNUSED(data);
}

int StopFunc(void *data)
{
	printf("Stopping task\n");
	SchedStop((sched_t *)data);
	return 0;
}

int AddTaskFunc(void *data)
{
	sched_t *sch = (sched_t *)data;
	
	printf("adding task1\n");
	SchedAddTask(sch, &StopFunc, time(NULL), 1, sch, &CleanFunc, NULL);
	return 0;
}

int AddTaskFunc2(void *data)
{
	sched_t *sch = (sched_t *)data;
	
	printf("adding task2\n");
	SchedAddTask(sch, &AddTaskFunc, time(NULL), 1, sch, &CleanFunc, NULL);
	return 0;
}

int RemoveFunc(void *data)
{
	rem_t rem_str = *(rem_t *)data;
	
	SchedRemove(rem_str.sch, rem_str.u_id);
	printf("Print task has removed\n");
	return 0;
}

int RemoveFunc2 (void *data)
{
	SchedRemove((sched_t *)data, test7_uid);
	printf("task has removed\n");
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
	/*---------------- Test variables decleration ----------------*/
	char *fail_message = NULL;
	sched_t *sched  = SchedCreate();
	/*------------------------------------------------------------*/
	
	time_t time_now = time(NULL);
	size_t interval_in_sec = 1;
	void *param = sched;
	void *clean_params = NULL;
	ilrd_uid_t uid;
	
	rem_t task_rem = {0};
	/*---------------- Test #1 ----------------*/
	fail_message = "Test 1 failed - "BOLD_RED"ScheIsEmpty"RESET;
	PrintTestResult(fail_message, SchedIsEmpty(sched), 1, __LINE__); 
	
	/*---------------- Test #2 ----------------*/
	SchedAddTask(sched, &FinishActionFunc, time_now + 1, 1, NULL, &CleanFunc , NULL);
	SchedAddTask(sched, &UnfinishActionFunc, time_now + 5, 1, NULL, &CleanFunc , NULL);
	fail_message = "Test 2 failed - "BOLD_RED"SchedAddTask"RESET;
	PrintTestResult(fail_message, SchedSize(sched), 1, __LINE__);
	
	SchedRun(sched);

	/*---------------- Test #3 ----------------*/
	/* SchedAddTask(sched, &FActionFunc, time_now, interval_in_sec, param
								,&CleanFunc , clean_params);
	SchedClear(sched);
	fail_message = "Test 3 failed - "BOLD_RED"SchedClear"RESET;
	PrintTestResult(fail_message, SchedIsEmpty(sched), 1, __LINE__); */
	
	/*---------------- Test #4 ----------------*/
	/* uid = SchedAddTask(sched, &ActionFunc, time_now, interval_in_sec, param
								,&CleanFunc , clean_params);
	fail_message = "Test 4 failed - "BOLD_RED"SchedRemove"RESET;
	PrintTestResult(fail_message, SchedRemove(sched, uid), 0, __LINE__); */
	
	/*---------------- Test #5 ----------------*/
	/* uid = SchedAddTask(sched, &ActionFunc, time_now, interval_in_sec, param, &CleanFunc , clean_params);
	
	task_rem.sch = sched;
	task_rem.u_id = uid;
	
	SchedAddTask(sched, &RemoveFunc, time_now + 2, interval_in_sec, (void *)&task_rem, &CleanFunc , clean_params);
	
	SchedAddTask(sched, &StopFunc, time_now + 5, interval_in_sec, param
								,&CleanFunc , clean_params);	
								
	fail_message = "Test 5 failed - "BOLD_RED"SchedRun"RESET;
	PrintTestResult(fail_message, SchedRun(sched), 1, __LINE__); */
	
	/*---------------- Test #6 ----------------*/
	/* SchedAddTask(sched, &ActionFunc, time(NULL), 2, param, &CleanFunc , clean_params);
	SchedAddTask(sched, &AddTaskFunc2, time(NULL) + 1, interval_in_sec, (void *) sched, &CleanFunc , clean_params);
			
	fail_message = "Test 6 failed - "BOLD_RED"SchedRun"RESET;
	PrintTestResult(fail_message, SchedRun(sched), 1, __LINE__); */
	
	/*---------------- Test #7 ----------------*/
	/* test7_uid = SchedAddTask(sched, &RemoveFunc2, time(NULL) + 1, interval_in_sec, (void *) sched, &CleanFunc , clean_params);
			
	fail_message = "Test 7 failed - "BOLD_RED"SchedRun"RESET;
	PrintTestResult(fail_message, SchedRun(sched), 0, __LINE__); */
	
	/*---------------- Destroy ----------------*/
	SchedDestroy(sched);
	
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
		 		printf(BOLD_GREEN"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣴⣦⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣴⣶⣿⣿⣿⣿⣿⣿⣿⣷⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⡿⠿⠿⠿⠿⣿⣿⡂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⠟⠉⠀⠐⠀⠀⠀⠀⠈⢿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⢿⣿⣿⡆⠀⢀⣠⣄⣤⣀⡄⣀⣀⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣎⠀⠀⣾⡷⣯⣿⡟⠀⢿⣯⣍⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡏⡏⡛⠇⠀⠀⠀⠈⠉⢈⡠⢀⣈⠍⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣥⣷⣋⢸⣆⠀⠀⢀⠠⢟⣻⣿⣟⡃⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢣⣷⡎⣿⣿⣾⡁⣴⣿⣿⢿⠿⣷⡀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠈⠁⢹⣿⣿⣿⣿⣿⣾⣶⡆⣼⡵⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⢠⢸⣿⣿⣿⣿⣿⣿⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡤⠀⠀⢣⣾⣿⢿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣮⢻⣷⣄⠀⢉⡞⣯⢷⡿⣿⣭⡀⠀⢤⠄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡴⣿⣿⣿⣷⣝⢿⣆⠰⢨⢅⠂⡹⣆⡍⡀⢀⣎⣮⢱⣉⢆⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣐⣾⣟⣾⣿⣿⣿⣿⣿⣶⣿⣿⣧⣎⣧⠀⢡⠆⢀⣰⡽⣯⣿⣟⣮⣶⣩⣆⡀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣔⣺⡽⣳⣯⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣟⣿⣽⣟⣿⣻⣿⣷⣾⠰⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⢀⣜⣳⣞⣷⣿⣿⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⢾⡽⣟⣿⣯⣿⣽⣿⣯⣿⣞⣷⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⢀⠾⣼⣳⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣟⣿⣯⣟⣿⣷⣿⢾⣷⣿⣟⣾⣟⣾⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⡿⣾⣻⣷⣿⣿⣿⣾⣿⣷⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⣀⠒⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣽⣿⢿⣿⣷⣻⣿⣾⣷⡬⡪⡴⢎⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠬⠁⢃⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠹⠿⢛⣩⣽⣾⣿⣻⣞⣷⣻⣷⣿⣄⠻⢅⣼⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠸⣦⡀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣾⣿⣿⣿⣽⣿⣽⣿⣳⢿⣿⣾⣿⣿⣺⢿⣿⣿⣇⢀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⡸⣦⣀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣷⣯⢿⣟⣾⣿⣿⣿⣻⣿⣿⣣⣷⡄⠀⠀⠀⠀⠀\n");
    printf("⠀⣰⣻⡽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣁⠤⠀⠂⠀⠀\n");
    printf("⠠⢿⣼⣿⣿⣿⣿⣿⣿⣿⠟⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢠⣌⠹⡟⢛⠛⡇⠺⢉⠉⣎⢋⢈⡻⡉⠏⢹⣿⣿⣷⣿⣍⡶⠆⠈⢀⠆\n");
    printf("⢻⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠨⣍⠰⡀⢿⠇⠁⢀⠻⡆⠿⢸⠸⢇⠤⠆⣿⣿⣿⣿⣿⣽⡴⠆⠐⠁⠤\n");
    printf("⠸⡭⢟⣿⣿⣿⣿⣿⣇⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣿⣶⣷⣶⠶⠾⠚⠓⠚⢒⣚⣓⣾⣶⣿⣿⣿⣿⣿⣿⣿⣷⣗⡈⢀⠀\n");
    printf("⠀⠑⣫⣾⣿⣿⣿⣿⣿⣆⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⡿⣿⣿⣿⣿⣿⠻⠿⠿⠿⠿⠿⠿⠟⠋⠀⠀\n");
    printf("⠀⠀⠐⠝⣾⣿⣿⣿⣿⣿⣆⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠈⢻⣞⣿⣿⢿⣿⣿⣧⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⣿⣿⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠹⣟⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣿⢿⣿⣻⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⡟⠉⠀⡀⠀⠱⣍⠛⢛⣛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣾⣹⢿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠙⠻⣿⣾⠏⠀⣤⣄⡑⣦⡄⠈⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣿⢷⣿⣽⣿⡙⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠱⡀⠰⣦⣙⡻⣿⣿⣿⣦⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣻⢿⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⢮⣝⠻⠭⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⡿⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠊⠙⠛⠚⠋⠛⠛⠛⠛⠛⠛⠛⠛⠛⠙⠛⠛⠙⠛⠛⠛⠃⠉⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"RESET);
	}
	return 0;
}


