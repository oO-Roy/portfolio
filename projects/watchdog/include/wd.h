#ifndef __WD_H__
#define __WD_H__

#include <stddef.h>     /* for size_t type */
#include <time.h>       /* for time_t type */

/* SIGUSR1 & SIGUSR2 signals are reserved for the watchdog exclusively 
and are not to be used by the client program*/

/* enum wd status description
    INIT_FAIL - returned when a failure occured during the initialization of WD.
                includes failure of system calls such as: fork, exec, pthread_create.
    TERMINATION_FAIL - returned when a failure occured at program termination.
    MEM_ALLOC_FAIL - memory allocation failure. 
    OTHER_FAIL - *to be specified at a later time when we will incounter more error posibilities.*
*/
typedef enum wd_status
{
    WD_SUCCESS = 0,
    WD_INIT_FAIL = 1,
    WD_TERMINATION_FAIL = 2,
    WD_MEM_ALLOC_FAIL = 3,
    WD_OTHER_FAIL = 4
} wd_status_t;

/* -----------------------------------------------------------------------------
    @ description: activates the watchdog. 
                the watchdog protects the program from failure, 
                upon failure, program is restarted.
    @ params:   interval_in_sec - time interval in seconds.
                intervals_per_check - the amount of intervals between watchdog checkups.
                argc and argv should be the client's main arguments.
*/
wd_status_t WDStart(time_t interval_in_sec, size_t intervals_per_check, 
                                                        int argc, char* argv[]);

/* -----------------------------------------------------------------------------
    @ description: this function deactivates the watchdog
*/
wd_status_t WDStop(void);

#endif /*__WD_H__*/  