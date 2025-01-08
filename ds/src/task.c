#include <assert.h> /* assert */
#include <stdlib.h> /* malloc & free */

#include "task.h"

struct task
{
	ilrd_uid_t uid;	
	time_t time_to_run;
	size_t interval_in_sec;
	task_action_func action_func;
	void *param;
	task_clean_up_func clean_up_func;
	void *clean_params;
};

task_t *TaskCreate(task_action_func action
					, time_t time_to_run
					, size_t interval_in_sec
					, void *param
					, task_clean_up_func clean
					, void *clean_params
					, ilrd_uid_t uid)
{
	task_t *task = NULL;
	
	assert(NULL != action);
	assert(NULL != clean);

	task = (task_t *) malloc(sizeof(task_t));
	if (NULL == task)
	{
		return NULL;
	}
	
	task->uid = uid;
	task->time_to_run = time_to_run;
	task->interval_in_sec = interval_in_sec;
	task->action_func = action;
	task->param = param;
	task->clean_up_func = clean;
	task->clean_params = clean_params;
	
	return task;
}

void TaskDestroy(task_t *task)
{
	if (NULL != task)
	{
		task->clean_up_func (task->clean_params);
		free(task);
	}
}

ilrd_uid_t GetUid(const task_t *task)
{
	assert(NULL != task);
	
	return task->uid;
}

int TaskRun(const task_t *task)
{
	assert(NULL != task);
	
	return (task->action_func (task->param));
}

time_t TaskGetTimeToRun(const task_t *task)
{
	assert(NULL != task);
	
	return task->time_to_run;
}

void TaskSetTimeToRun(task_t *task)
{
	assert(NULL != task);
	
	task->time_to_run = (time(NULL) + task->interval_in_sec);
}
