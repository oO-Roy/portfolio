#include "tcp_lib.h"

int main(int argc, char const *argv[])
{
    int status = 0;
    
    if (2 == argc)
    {
        status = TCPServer(argv[1]);
    }
    
    return status;
}
