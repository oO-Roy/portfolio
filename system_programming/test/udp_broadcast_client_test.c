#include "udp_lib.h" /*API*/

int main(int argc, char *argv[])
{
    int status = 0;
    
    if (2 == argc)
    {
        status = UDPBroadcastClient(argv[1]);
    }
    return status;
}
