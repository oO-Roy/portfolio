#include <unistd.h> /* getpid */

#include "uid.h" /* typedef ilrd_uid_t, time */

#define INVALID_TIME_STAMP ((time_t) - 1)

ilrd_uid_t UIDCreate(void)
{
	static atomic_uint_fast64_t counter = ATOMIC_VAR_INIT(0);
	
	ilrd_uid_t identifier = {0};
	
	identifier.counter = atomic_fetch_add(&counter, 1);
	identifier.pid_stamp = getpid();
	identifier.time_stamp = time(NULL);
	if (INVALID_TIME_STAMP == identifier.time_stamp)
	{
		identifier.counter = 0;
		identifier.pid_stamp = 0;
	}
	
	return identifier;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	return ((uid1.counter == uid2.counter) 
			&& (uid1.pid_stamp == uid2.pid_stamp)
				&& (uid1.time_stamp == uid2.time_stamp));
}

ilrd_uid_t UIDBadUID(void)
{
	ilrd_uid_t identifier = {INVALID_TIME_STAMP, 0, 0};
	
	return identifier;
}
