#include "server.h"

int main(int argc, char *argv[])
{
    int status = 0;
    
    if (3 == argc)
    {
        status = UDPClient(argv[1], argv[2]);
    }
    
    return status;
}