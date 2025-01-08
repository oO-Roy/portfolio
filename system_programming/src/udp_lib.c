#include <string.h> /*strlen*/

#include "udp_lib.h"

static int CreateUDPSocket(sain_t* server_address, const uint16_t port)
{
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (0 > socket_fd)
    {
        return -1;
    }

    server_address->sin_family = AF_INET;
    server_address->sin_port = htons(port);
    server_address->sin_addr.s_addr = INADDR_ANY;

    return socket_fd;
}

static int ReciveMessage(const int socket_fd, sa_t* addr, socklen_t add_len)
{
    int num_bytes = 0;
    char buffer[BUFFER_SIZE] = {0};

    num_bytes = recvfrom(socket_fd, buffer, (BUFFER_SIZE - 1), 0
                                                            , addr, &add_len);
    if (ERROR == num_bytes)
    {
        return ERROR;
    }

    buffer[num_bytes] = '\0';
    printf("%s\n", buffer);

    return 0;
}

/******************************************************************************/
int UDPServer(const char* port)
{
    int udp_fd = 0;
    sain_t server_addr = {0};
    sain_t client_addr = {0};
    socklen_t client_len = sizeof(client_addr);

    udp_fd = CreateUDPSocket(&server_addr, atoi(port));
    if (ERROR == udp_fd)
    {
        return ERROR;
    }

    if (0 != bind(udp_fd, (sa_t*)&server_addr, sizeof(server_addr)))
    {
        return ERROR;
    }

    while (1)
    {
        if (ERROR == ReciveMessage(udp_fd, (sa_t*)&client_addr, client_len))
        {
            return ERROR;
        }

        if (ERROR == sendto(udp_fd, "Pong", 5, 0, (sa_t*)&client_addr
                                                                , client_len))
        {
            return ERROR;
        }

        sleep(1);
    }
    
    close(udp_fd);

    return 0;
}

int UDPClient(const char* address, const char* port)
{
    int i = 10;
    int udp_fd = 0;
    sain_t server_addr = {0};
    sain_t client_addr = {0};

    udp_fd = CreateUDPSocket(&server_addr, atoi(port));
    if (ERROR == udp_fd)
    {
        return ERROR;
    }

    server_addr.sin_addr.s_addr = inet_addr(address);

    if (0 > connect(udp_fd, (sa_t*)&server_addr, sizeof(server_addr)))
    {
        return ERROR;
    }
    
    while (i--)
    {
        if (ERROR == sendto(udp_fd, "Ping", 5, 0, (sa_t*)&server_addr
                                                        , sizeof(server_addr)))
        {
            return ERROR;
        }
        
        if (ERROR == ReciveMessage(udp_fd, (sa_t*)&client_addr
                                                        , sizeof(client_addr)))
        {
            return ERROR;
        }

        sleep(1);
    }
    
    close(udp_fd);

    return 0;    
}

int UDPBroadcastServer(const char* port, const char* message)
{
    int broadcast = 1;
    int udp_broadcast_fd = 0;
    sain_t broadcast_addr = {0};
    socklen_t broadcast_len = sizeof(broadcast_addr);

    udp_broadcast_fd = CreateUDPSocket(&broadcast_addr, atoi(port));
    if (ERROR == udp_broadcast_fd)
    {
        return ERROR;
    }
    
    broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    if (ERROR == setsockopt(udp_broadcast_fd, SOL_SOCKET, SO_BROADCAST
                                            , &broadcast, sizeof(broadcast)))
    {
        return ERROR;
    }

    while (1)
    {
        if (ERROR == sendto(udp_broadcast_fd, message, strlen(message), 0
                                    , (sa_t*)&broadcast_addr, broadcast_len))
        {
            return ERROR;
        }

        if (!strcmp(message, "ping"))
        {
            if (ERROR == ReciveMessage(udp_broadcast_fd, (sa_t*)&broadcast_addr
                                                            , broadcast_len))
            {
                return ERROR;
            }
        }

        sleep(1);
    }

    close(udp_broadcast_fd);

    return 0;
}

int UDPBroadcastClient(const char* port)
{
    int broadcast = 1;
    int udp_broadcast_fd = 0;
    sain_t broadcast_addr = {0};
    socklen_t broadcast_len = sizeof(broadcast_addr);

    udp_broadcast_fd = CreateUDPSocket(&broadcast_addr, atoi(port));
    if (ERROR == udp_broadcast_fd)
    {
        return ERROR;
    }

    broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    if (ERROR == setsockopt(udp_broadcast_fd, SOL_SOCKET, SO_REUSEADDR
                                            , &broadcast, sizeof(broadcast)))
    {
        return ERROR;
    }

    if (ERROR == bind(udp_broadcast_fd, (sa_t*)&broadcast_addr, broadcast_len))
    {
        return ERROR;
    }

    while (1)
    {
        if (ERROR == ReciveMessage(udp_broadcast_fd, (sa_t*)&broadcast_addr
                                                            , broadcast_len))
        {
            return ERROR;
        }

        sleep(1);
    }

    close(udp_broadcast_fd);

    return 0;
}