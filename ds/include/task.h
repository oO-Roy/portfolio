#ifndef __TASK_H__
#define __TASK_H__

#include "uid.h" /* typedef ilrd_uid_t, API use */
#include <time.h> /* time_t */

typedef struct task task_t;

typedef int (*task_action_func)(void *param);
typedef void (*task_clean_up_func)(void *param);

task_t *TaskCreate(task_action_func action
					, time_t time_to_run
					, size_t interval_in_sec
					, void *param
					, task_clean_up_func clean
					, void *clean_params
					, ilrd_uid_t uid);
					
void TaskDestroy(task_t *task);

ilrd_uid_t GetUid(const task_t *task);

int TaskRun(const task_t *task);

time_t TaskGetTimeToRun(const task_t *task);
void TaskSetTimeToRun(task_t *task);

#endif	/* __TASK_H__ */
