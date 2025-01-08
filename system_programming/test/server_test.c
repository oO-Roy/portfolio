#include "server.h"

int main(int argc, char *argv[])
{
	int status = 0;
    
    if (2 == argc)
    {
        status = Server(argv[1]);
    }
    
    return status;
}
