#define _POSIX_C_SOURCE 199809L
#include <stdio.h>     /*printf*/
#include <signal.h>    /*SIGUSR1, SIGUSR2*/
#include <sys/types.h> /*pid_t*/
#include <unistd.h> /*fork & exec*/

#define UNUSED(x) (void)(x)
#define ARGV_SIZE 10

void PingHandler(int signal)
{
	UNUSED(signal);
	sleep(1);
	printf("ping\n");
}

int main()
{
    struct sigaction sa = {0};
    pid_t child_pid = 0;
	char* argv[ARGV_SIZE] = {0};

	sa.sa_handler = &PingHandler;
	sigaction(SIGUSR2, &sa, NULL);
	
	child_pid = fork();
	if (0 > child_pid)
	{
		return -1;
	}
	
	else if (0 == child_pid)
	{
		execvp("./run_pong", argv);
	}

	else
	{
		while (1)
		{
			pause();
			kill(child_pid, SIGUSR1);
		}
	}

	return 0;
}
