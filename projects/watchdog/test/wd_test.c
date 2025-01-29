#include <stdio.h> /*printf*/
#include <unistd.h> /*getpid*/

#include "wd.h"

#define UNUSED(x) (void)(x)

int main(int argc, char *argv[])
{
	time_t now = 0;
	time_t start = time(NULL);

	printf("Hey I'm CLIENT[%d]\n", getpid());

	WDStart(1, 2, argc, argv);

	/*start of reviving section*/
	while (now < start + 15)
	{
		now = time(NULL);
	}

	/*end of reviving section*/
	printf("WDStop status: %d\n", WDStop());

	return 0;
}
