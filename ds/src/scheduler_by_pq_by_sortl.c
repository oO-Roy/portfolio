/* 
	Developer: Roy Steiner.
	File: Scheduler.
	Reviewer: Amos Duek. 
	Date: 20/07/24
	Status: Approved.
*/
#include <unistd.h> /* sleep */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc and free */

#include "scheduler_by_pq_by_sortl.h" /* typedef sche_t, */
#include "pq.h" /* container of scheduler and API use */
#include "task.h" /* forward declaration */

#define EXTRACT_PRIORITY_QUEUE(x) (x->p_queue)

struct scheduler
{
	pq_t *p_queue;
	int is_running;
};

int IsMatch(const void *data, void *param)
{
	return UIDIsSame(GetUid((task_t *)data), *(ilrd_uid_t *)param);
}

int CompFunc(const void *a, const void *b)
{
	return (int)(difftime(TaskGetTimeToRun((task_t *)b),TaskGetTimeToRun((task_t *)a)));
}

sched_t *SchedCreate(void)
{
	sched_t *sched = NULL;

	sched = (sched_t *) malloc(sizeof(sched_t));
	if (NULL == sched)
	{
		return NULL;
	}
	
	EXTRACT_PRIORITY_QUEUE(sched) = PQCreate(&CompFunc);
	if (NULL == EXTRACT_PRIORITY_QUEUE(sched))
	{
		free(sched);
		return NULL;
	}
	
	sched->is_running = 0;
	
	return sched;
}

size_t SchedSize(const sched_t *sched)
{
	assert(NULL != sched);
	
	return PQSize(EXTRACT_PRIORITY_QUEUE(sched));
}

int SchedIsEmpty(const sched_t *sched)
{
	assert(NULL != sched);
	
	return PQIsEmpty(EXTRACT_PRIORITY_QUEUE(sched));
}

ilrd_uid_t SchedAddTask(sched_t *sched, sched_action_func action
							, time_t time_to_run
							, size_t interval_in_sec, void *param
							, sched_clean_up_func clean, void *clean_params)
{
	task_t *task = NULL;
	ilrd_uid_t valid_uid = UIDCreate();
	ilrd_uid_t bad_uid = UIDBadUID();
	
	assert(NULL != sched);
	assert(NULL != action);
	assert(NULL != clean);
	
	if (UIDIsSame(valid_uid, bad_uid))
	{
		return bad_uid;
	}
	
	task = TaskCreate(action, time_to_run, interval_in_sec, param, clean
												, clean_params, valid_uid);
	if (NULL == task)
	{
		return bad_uid;
	}
															
	if (PQEnqueue(EXTRACT_PRIORITY_QUEUE(sched), (void *)task))
	{
		TaskDestroy(task);
		return bad_uid;
	}
	
	return valid_uid;
} 

void SchedClear(sched_t *sched)
{
	task_t *task = NULL;
	
	assert(NULL != sched);
	
	while (!SchedIsEmpty(sched))
	{
		task = PQDequeue(EXTRACT_PRIORITY_QUEUE(sched));
		TaskDestroy(task);
	}
}

void SchedDestroy(sched_t *sched)
{
	assert(NULL != sched);
	
	SchedClear(sched);
	PQDestroy(EXTRACT_PRIORITY_QUEUE(sched));
	free(sched);
}

int SchedRemove(sched_t *sched, ilrd_uid_t uid)
{
	task_t *task = NULL;
	
	assert(NULL != sched);
	
	task = PQErase(EXTRACT_PRIORITY_QUEUE(sched), &IsMatch, &uid);
	if (NULL == task)
	{
		return 1;
	}
	
	TaskDestroy(task);
	
	return 0;
}

int SchedRun(sched_t *sched)
{
	task_t *task = NULL;
	ilrd_uid_t task_uid = {0};
	ilrd_uid_t uid = {0};
	int task_status = 0;
	time_t remain_time = 1;
		
	assert(NULL != sched);
	
	sched->is_running = 0;
	
	while (!SchedIsEmpty(sched) && !sched->is_running)
	{
		task = (task_t *) PQPeek(EXTRACT_PRIORITY_QUEUE(sched));
		
		if (time(NULL) >= TaskGetTimeToRun(task))								
		{
			task_uid = GetUid(task);
			task_status = TaskRun(task);
			
			uid = GetUid((task_t *)PQPeek(EXTRACT_PRIORITY_QUEUE(sched)));
			
			if (UIDIsSame(uid, task_uid))
			{
				task = PQDequeue(EXTRACT_PRIORITY_QUEUE(sched));
			}
			else
			{
				task = PQErase(EXTRACT_PRIORITY_QUEUE(sched), &IsMatch, &uid);
			}
			
			if (1 == task_status)
			{
				TaskSetTimeToRun(task);
				if (PQEnqueue(EXTRACT_PRIORITY_QUEUE(sched), (void *)task))
				{
					return -1;
				}
			}
			else
			{
				TaskDestroy(task);
			}
		}
		else
		{	
			remain_time = sleep(TaskGetTimeToRun(task) - time(NULL));
			
			while (remain_time)
			{
				remain_time = sleep(remain_time);
			}
		}
		sleep(1);
	}
	
	return sched->is_running;
}

void SchedStop(sched_t *sched)
{
	assert(NULL != sched);
	
	sched->is_running = 1;
}
