#include "udp_lib.h" /*API*/

int main(int argc, char *argv[])
{
    int status = 0;
    
    if (3 == argc)
    {
        status = UDPBroadcastServer(argv[1], argv[2]);
    }
    return status;
}
