#ifndef __UID_H__
#define __UID_H__

#include <stddef.h> /* size_t */
#include <time.h>	/* time_t */
#include <sys/types.h> /* pid_t */
#include <stdatomic.h>  /*atomic operations on counter*/

/* This API is thread safe */

typedef struct ilrd_uid
{ 	
	time_t time_stamp;
	pid_t pid_stamp;
	atomic_uint_fast64_t counter;
}ilrd_uid_t;

/* Description: the function creates a unique identifier.
   Returns: unique identifier.
   Error: returns invalid identifier.
   Time complexity: O(1) */
ilrd_uid_t UIDCreate(void);

/* Description: the function checks if a unique identifier #1 
														is the same as #2
   Returns: 1 on success , 0 otherwise.
   Error: none. 
   Time complexity: O(1) */
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2);

/* Description: the function creates an invalid indentifier. 
   Returns: invalid indentifier.
   Error: none.
   Time complexity: O(1). */
ilrd_uid_t UIDBadUID(void);

#endif	/* __UID_H__ */
