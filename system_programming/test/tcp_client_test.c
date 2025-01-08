#include "server.h"

int main(int argc, char const *argv[])
{
    int status = 0;
    if (3 == argc)
    {
        status = TCPClient(argv[1], argv[2]);
    }
    
    return status;
}
