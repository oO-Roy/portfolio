#include "udp_lib.h"

int main(int argc, char const *argv[])
{
    int status = 0;
    
    if (2 == argc)
    {
        status = UDPServer(argv[1]);
    }
    
    return status;
}
