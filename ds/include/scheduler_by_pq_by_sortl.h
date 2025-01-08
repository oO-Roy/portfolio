#ifndef __SHCEDULER_BY_PQ_BY_SORTL_H__
#define __SHCEDULER_BY_PQ_BY_SORTL_H__

#include <stddef.h> /* size_t */
#include "uid.h" /* typedef UID DS, time_t */

typedef struct scheduler sched_t;

/*
    @ Description: Performs a certain action on a specified parameter.
    @ Params: param - A pointer to the parameter on which the action should be performed on.
    @ Return value: 0 - done, 1 - not done, -1 - error.
*/
typedef int (*sched_action_func)(void *param);

/*
    @ Description: frees all memory allocations performed by action_func.
    @ Params: param - A pointer to the parameter on which the clean should be performed on.
    @ Return value: none.
*/
typedef void (*sched_clean_up_func)(void *param);

/*
    @ description: Creates a scheduler.
    @ params: void
    @ return value: sched_t * - A pointer to the created scheduler.
    @ errors: Returns NULL if memory allocation failed.
    @ time complexity: O(1). 
*/
sched_t *SchedCreate(void);

/*
    @ description: Frees all memory allocations.
    @ params: sche - A pointer to the scheduler to destroy.
    @ return value: None.
    @ errors: None.
    @ time complexity: O(n).
*/
void SchedDestroy(sched_t *sched);

/*
    @ description: Creates a new task in the scheduler.
    @ params: sche - scheduler to insert the new task into, 
              action - action function which the task should perform, 
              time_to_run - when the task should start,
              interval_in_sec - the time between each invocation of the task, 
              param - A pointer to the parameter on which the action is performed on, 
              clean - function that frees all memory allocations performed by action_func, 
              clean_params - A pointer to the parameter on which the clean function should be performed on.
    @ return value: Task uid.
    @ errors: Returns bad uid if memory allocation fails.
    @ time complexity: O(1).
*/
ilrd_uid_t SchedAddTask(sched_t *sched, sched_action_func action, time_t time_to_run,
                    size_t interval_in_sec, void *param, 
                    sched_clean_up_func clean, void *clean_params);

/*
    @ description: Removes a task from the scheduler.
    @ params: sche - A pointer to the scheduler data structure.
              uid - the uid of the task to be removed. 
    @ return value: 0 - task removed, 1 - task not found.
    @ errors: In case of an empty schedueler, behavior is undefined.
    @ time complexity: O(n)
*/
int SchedRemove(sched_t *sched, ilrd_uid_t uid);

/*
    @ description: Every second executes a task in the scheduler. each task would be executed not before its time_to_run.
    @ params: sche - A pointer to the scheduler data structure.
    @ return value: 0 - all tasks completed, 1 - the scheduler activity stoped, -1 allocation memory fail.
    @ errors: some task failed.
    @ time complexity: unknown. not less than O(n).
*/
int SchedRun(sched_t *sched);

/*
    @ description: Stops the execution of the tasks in the scheduler.
    @ params: sche - A pointer to the scheduler data structure.
    @ return value: None.
    @ errors: 
    @ time complexity: O(1). 
*/
void SchedStop(sched_t *sched);

/*
    @ description: Checks the number of tasks in the scheduler.
    @ params: sche - A pointer to the scheduler data structure.
    @ return value: The number of tasks in the scheduler.
    @ errors: None
    @ time complexity: O(n).
*/
size_t SchedSize(const sched_t *sched);

/*
    @ description: Checks whether the scheduler is empty of tasks.
    @ params: sche - A pointer to the scheduler data structure.
    @ return value: non-zero if empty, 0 otherwise.
    @ errors: 
    @ time complexity: O(1).
*/
int SchedIsEmpty(const sched_t *sched);

/*
    @ description: Clears the scheduler from all tasks. 
    @ params: sche - A pointer to the scheduler data structure.
    @ return value: None.
    @ errors: None.
    @ time complexity: O(n).
*/
void SchedClear(sched_t *sched);

#endif /* __SHCEDULER_BY_PQ_BY_SORTL_H__ */
