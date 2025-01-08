#ifndef NDEBUG
	#include <stdio.h> /*printf*/
#endif
#include <stdlib.h> /*atoi*/
#include <unistd.h> /*getpid*/

#include "wd.h" /*API**/

int main(int argc, char *argv[])
{	
	size_t interal = atoi(argv[argc - 2]);
	size_t trashold = atoi(argv[argc - 1]);
	
	#ifndef NDEBUG
	printf(" Hey there I'm WATCHDOG [pid: %d]\n", getpid());
	#endif

	WDStart(interal, trashold, argc, argv);

	return 0;
}