/* 
	Developer: Roy Steiner.
	File: Watchdog.
	Reviewer: Evyatar Wienberg.
	Date: 28/09/2024.
	Status: APPROVED.
*/
#define _POSIX_C_SOURCE 200112L /* signal.h & envp funcs */

#ifndef NDEBUG
	#include <stdio.h> /*printf*/
#endif
#include <stdlib.h> /*setenvp, getenvp, unsetenvp, atoi*/
#include <stdatomic.h> /*typedef*/
#include <pthread.h> /*create*/
#include <signal.h> /*sigaction, struct sigaction*/
#include <semaphore.h> /*semaphore*/
#include <fcntl.h>  /* For O_* constants */
#include <unistd.h> /*fork & exec*/
#include <assert.h> /*assert*/

#include "wd.h" /*wd_status_t*/
#include "scheduler.h" /*API use*/

#define UNUSED(x) (void)(x)
#define RW_R_R 0644

static int IsWatchdogProcess(void);
static wd_status_t CreateWDProcess();

/*------------------------------GLOBAL VARIABLES------------------------------*/
static atomic_uint_fast64_t g_counter;
static pthread_t g_thread_id;
static size_t g_trashold;
static pid_t g_child_pid;
static char* g_argv_thread[BUFSIZ];
static int g_is_stop;
static sched_t* sched;
static int g_status;

static sem_t unnamed_sem;
static sem_t* first_sem;
static sem_t* second_sem;

static char interval_buff[BUFSIZ];
static char trashold_buff[BUFSIZ];

typedef struct thread_params
{
    time_t interval_in_sec;
    size_t intervals_per_check;
}thread_params_t;

/*---------------------SCHEDULER ACTION & CLEAN FUNCTIONS---------------------*/
static void CleanFunc(void* param)
{
    UNUSED(param);
}

static int CheckTrashold(void* param)
{  
    UNUSED(param);

    if (g_counter >= g_trashold)
    {
        #ifndef NDEBUG
            IsWatchdogProcess() 
                ? printf("CLIENT [pid: %d] is DEAD %d\n", getppid(), getpid()) 
                : printf("  WATCHDOG [pid: %d] is DEAD %d\n", g_child_pid
                                                                    , getpid());
        #endif

         if (IsWatchdogProcess())
        {
            kill(getppid(), SIGUSR2); /*verify CLIENT is dead*/
            unsetenv("WD_PID");

            g_child_pid = fork();
            if (0 > g_child_pid)
            {
                g_status = WD_OTHER_FAIL;
                return 0;
            } 
            
            else if (0 == g_child_pid)
            {
                kill(getppid(), SIGUSR2); /*kill the reviving WD*/
                if (execv(g_argv_thread[0], g_argv_thread))
                {
                    g_status = WD_OTHER_FAIL;
                    return 0;
                }
            }
        }

        else /*IsClientProcess*/
        {
            kill(g_child_pid, SIGUSR2);

            if (NULL != getenv("WD_PID"))
            {
                unsetenv("WD_PID");
            }

            if (CreateWDProcess())
            {
                g_status = WD_OTHER_FAIL;
                return 0;
            }
        }
    }

    else
    {
        #ifndef NDEBUG
            IsWatchdogProcess() 
            ? printf("CLIENT [pid: %d] is ALIVE %d\n", getppid(), getpid()) 
                : printf("  WATCHDOG [pid: %d] is ALIVE %d\n", g_child_pid
                                                                    , getpid());
        #endif
    }    

    return 1;
}

static int SendSignal(void* param)
{
    UNUSED(param);
    IsWatchdogProcess() ? kill(getppid(), SIGUSR1) : kill(g_child_pid, SIGUSR1);
    atomic_fetch_add(&g_counter, 1);

    return 1;
}

/*-------------------------------SIGNAL HANDLERS------------------------------*/
void SIGUSR1Handler(int signal)
{
    UNUSED(signal);
    atomic_store(&g_counter, 0);
}

void SIGUSR2Handler(int signal)
{
    UNUSED(signal);
    SchedStop(sched);
    sem_destroy(&unnamed_sem);
    sem_close(first_sem);
    sem_close(second_sem);
    sem_unlink("/first_sem");
    sem_unlink("/second_sem");
    unsetenv("WD_PID");

    if (g_is_stop)
    {
        g_is_stop = 0;
        kill(getppid(), SIGUSR2);
    }
}

/*------------------------------STATIC FUNCTIONS------------------------------*/
static void ExitFunc(void)
{
    pthread_join(g_thread_id, NULL);
}

static wd_status_t Init(thread_params_t* params, time_t interval_in_sec
    , size_t intervals_per_check, int argc, char* argv[], struct sigaction* sa)
{
    int i = 0;
    
    g_trashold = intervals_per_check;

    sprintf(interval_buff, "%ld", interval_in_sec);
    sprintf(trashold_buff, "%ld", intervals_per_check);

    for (; i < argc; ++i)
    {
        g_argv_thread[i] = argv[i];
    }
    
    g_argv_thread[i++] = interval_buff;
    g_argv_thread[i++] = trashold_buff;
    g_argv_thread[i] = NULL;

    params->interval_in_sec = interval_in_sec;
    params->intervals_per_check = intervals_per_check;

    sa->sa_handler = &SIGUSR1Handler;
    if (sigaction(SIGUSR1, sa, NULL))
    {
        return WD_INIT_FAIL;
    }

    sa->sa_handler = &SIGUSR2Handler;
    if (sigaction(SIGUSR2, sa, NULL))
    {
        return WD_INIT_FAIL;
    }

    if (sem_init(&unnamed_sem, 0, 0))
    {
        return WD_INIT_FAIL;
    }

    first_sem = sem_open("/first_sem", O_CREAT, RW_R_R, 0);
    if (SEM_FAILED == first_sem)
    {
        return WD_INIT_FAIL;
    }
    
    second_sem = sem_open("/second_sem", O_CREAT, RW_R_R, 0);
    if (SEM_FAILED == second_sem)
    {
        return WD_INIT_FAIL;

    }
    
    if (IsWatchdogProcess())
    {
        if (atexit(ExitFunc))
        {
            return WD_INIT_FAIL;
        }
    }
    
    return WD_SUCCESS;
}

static wd_status_t InitScheduler(void* param)
{   
    thread_params_t* thread_param = (thread_params_t*)param;
    sched = SchedCreate();
    if (NULL == sched)
    {
        g_status = WD_INIT_FAIL;
        return WD_INIT_FAIL;
    }
    
    /* Send alive signal each interval */
    SchedAddTask(sched, &SendSignal, time(NULL), thread_param->interval_in_sec
    , param, &CleanFunc, NULL);

    /* Check if g_counter within trashold limit */
    SchedAddTask(sched, &CheckTrashold
    , time(NULL) + thread_param->intervals_per_check
    , (thread_param->interval_in_sec * thread_param->intervals_per_check)
    , param, &CleanFunc, NULL);

    return WD_SUCCESS;
}

static int IsWatchdogProcess(void)
{
    return (NULL != getenv("WD_PID") && atoi(getenv("WD_PID")) == getpid());
}

static void SetEnvPID(void)
{
    char buffer[BUFSIZ];

    sprintf(buffer, "%d", getpid());
    setenv("WD_PID", buffer, 0);
}

static wd_status_t CreateWDProcess()
{
    g_child_pid = fork();
    if (-1 == g_child_pid)
    {
        return WD_INIT_FAIL;
    } 

    else if (0 == g_child_pid)
    {
        SetEnvPID();
        
        if (execvp("./wd.out", g_argv_thread))
        {
            g_status = WD_INIT_FAIL;
            return WD_INIT_FAIL;
        }
    }

    sem_post(first_sem);
    sem_wait(second_sem);

    return WD_SUCCESS;
}

static void* ThreadWork(void* param)
{ 
    if (IsWatchdogProcess())
    {
        sem_wait(first_sem);
        sem_post(second_sem);
    }
    
    else if (CreateWDProcess())
    {
        g_status = WD_INIT_FAIL;
        sem_post(&unnamed_sem);
        return NULL;
    }
    
    if (InitScheduler(param))
    {
        g_status = WD_INIT_FAIL;
        sem_post(&unnamed_sem);
        return NULL;
    }
    
    sem_post(&unnamed_sem);
    SchedRun(sched);
    SchedDestroy(sched);

    return NULL;
}

/*-------------------------------API FUNCTIONS--------------------------------*/
wd_status_t WDStart(time_t interval_in_sec, size_t intervals_per_check
                                                    , int argc, char* argv[])
{
    thread_params_t params = {0};
    struct sigaction sa = {0};

    if (Init(&params, interval_in_sec, intervals_per_check, argc, argv, &sa))
    {
        return WD_INIT_FAIL;
    }
    
    if (pthread_create(&g_thread_id, NULL, &ThreadWork, &params))
    {
        return WD_INIT_FAIL;
    }

    if (sem_wait(&unnamed_sem))
    {
        return WD_OTHER_FAIL;
    }

    return g_status;
}

wd_status_t WDStop(void)
{   
    g_is_stop = 1;
    kill(g_child_pid, SIGUSR2);
    
    return WD_SUCCESS;
}