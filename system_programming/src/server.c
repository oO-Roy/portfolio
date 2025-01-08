#include <stdlib.h> /*exit, EXIT_FAILURE, atoi*/
#include <stdio.h> /*printf, scanf */
#include <string.h> /*memset, memcpy, strlen, strcmp*/
#include <errno.h> /*errno, EWWOULDBLOCK*/
#include <fcntl.h> /*fcntl, F_GETFL, F_SETFL, O_NONBLOCK*/
#include <arpa/inet.h> /*inet_addr*/
#include <unistd.h> /*read, send, close*/
#include <time.h> /*srand, rand*/
#include <netdb.h> /*struct sockaddr, struct sockaddr_in, spcklen_t, AF_INET
                                                        , INADDR_ANY, uint16_t*/
#include <sys/select.h> /*fd_set, struct timeval, select, FD_ISSET, FD_CLR
                                            , FD_SET, FD_ZERO, STDIN_FILENO*/

#include "server.h"
#include "logger.h"

#define QUIT "quit"
#define PING "ping"
#define PONG "pong"

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define BACKLOG 10
#define TIMEOUT_S 7
#define NUM_OF_PINGS 5
#define BUFF_SIZE 80

#define MAX2(a, b) ((a > b) * a + (b >= a) * b)

typedef struct sockaddr sa_t;
typedef struct sockaddr_in sain_t;

char logger[BUFF_SIZE];

/******************************************************************************/
static int RandInRange(int from, int to)
{
    srand(time(NULL));

    return (rand() % (to - from + 1)) + from;
}

static int STDINCommunication()
{
    int status = 0;
    char buffer[BUFF_SIZE];
    
    scanf("%s", buffer);

    if (!strcmp(buffer, PING))
    {
        printf(PONG"\n");
    }

    else if (!strcmp(buffer, QUIT))
    {
        LoggerInsertLine(logger, "quiting...");
        status = ERROR;
    }

    return status;
}

static int TCPClientCommunication(const int tcp_fd)
{
    char buffer[BUFF_SIZE] = {0};
    
    if (ECONNRESET == send(tcp_fd, PING, 5, 0))
    {
        LoggerInsertLine(logger, "session ended.");
        return ERROR;
    }

    LoggerInsertLine(logger, PING);

    recv(tcp_fd, buffer, BUFF_SIZE, 0);
    printf("%s\n", buffer);
    LoggerInsertLine(logger, buffer);

    
    return TRUE;
}

static void TCPServerCommunication(const int tcp_fd, fd_set* master_fds
                                                            , int* max_sd)
{
    char buffer[BUFF_SIZE] = {0};
    int close_connection = FALSE;
    int rc = recv(tcp_fd, buffer, BUFF_SIZE, 0);
    
    if (0 > rc)
    {
        if (errno != EWOULDBLOCK)
        {
            perror("recv");
            close_connection = TRUE;
        }
    }

    else if (0 < rc)
    {
        printf("%s\n", buffer);
    }

    else
    {
        LoggerInsertLine(logger, "Server: TCP connection closed.");
        close_connection = TRUE;
    }
    
    if (FALSE == close_connection)
    {
        rc = send(tcp_fd, PONG, 5, 0);
        if (0 > rc)
        {
            perror("send");
            close_connection = TRUE;
        }  
    }

    if (TRUE == close_connection)
    {
        close(tcp_fd);
        FD_CLR(tcp_fd, master_fds);
        if (tcp_fd == *max_sd)
        {
            while (FD_ISSET(*max_sd, master_fds))
            {
                --(*max_sd);
            }
        }
    }
}

static int UDPClientReciveMessage(const int socket_fd, sa_t* addr
                                                        , socklen_t add_len)
{
    int num_bytes = 0;
    char buffer[BUFF_SIZE] = {0};

    num_bytes = recvfrom(socket_fd, buffer, (BUFF_SIZE - 1), 0
                                                            , addr, &add_len);
    if (ERROR == num_bytes)
    {
        return ERROR;
    }

    buffer[num_bytes] = '\0';
    printf("%s\n", buffer);
    LoggerInsertLine(logger, buffer);

    return FALSE;
}

static void UDPServerCommunication(const int udp_fd, sain_t* client_addr
                                                        ,socklen_t* addr_len)
{
    ssize_t recv_return = 0;
    char buffer[BUFF_SIZE] = {0};
    
    recv_return = recvfrom(udp_fd, buffer, BUFF_SIZE, 0, (sa_t*)client_addr
                                                                    , addr_len);
    if (ERROR == recv_return)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", buffer);

    if (!strcmp(buffer, PING))
    {
        if (ERROR == sendto(udp_fd, PONG, 5, 0, (sa_t*)client_addr
                                                                , *addr_len))
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }
}

static int NewTCPConnectionsHandler(const int tcp_fd, fd_set* master_fds
                                                    , int* new_fd, int* max_sd)
{
    while (ERROR != *new_fd)
    {
        *new_fd = accept(tcp_fd, NULL, NULL);
        if (0 > *new_fd)
        {
            if (errno != EWOULDBLOCK)
            {
                perror("accept");
                return TRUE;   
            }
            break;
        }
        
        LoggerInsertLine(logger, "New TCP connection established");

        FD_SET(*new_fd, master_fds);
        if (*new_fd > *max_sd)
        {
            *max_sd = *new_fd;
        }
    }

    return FALSE;
}

static void InitFileDescriptorsSet(fd_set* fds, const int first_fd
                                    , const int second_fd, const int third_fd)
{
    FD_ZERO(fds);
    FD_SET(first_fd, fds);
    FD_SET(second_fd, fds);
    FD_SET(third_fd, fds);
}
 
static void ListenForConnections(const int socket_fd)
{
    if (ERROR == listen(socket_fd, BACKLOG))
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

static void BindServer(int socket_fd, sain_t* server_address)
{
    if (ERROR == bind(socket_fd, (sa_t*)server_address
                                                    , sizeof(*server_address)))
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

static void InitServerAddressStruct(sain_t* server_address
                                                        , const uint16_t port)
{
    memset(server_address, 0, sizeof(*server_address));
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = INADDR_ANY;
    server_address->sin_port = htons(port);
}

static void SetNonBlocking(int socket_fd)
{
    /* saving old file descriptor control*/
    int flags = fcntl(socket_fd, F_GETFL, 0);
    if (ERROR == flags)
    {
        perror("fcntl get");
        exit(EXIT_FAILURE);
    }

    if (ERROR == fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK))
    {
        perror("fcntl set");
        exit(EXIT_FAILURE);
    }
}

static int CreateServer(int type)
{
    int socket_fd = socket(AF_INET, type, 0);

    if (ERROR == socket_fd)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    SetNonBlocking(socket_fd);

    return socket_fd;
}

/******************************************************************************/
int Server(const char* port)
{
    int i =  0;
    int end_server = 0;
    int rc = 0;
    int max_sd = 0;
    int ready_fd = 0;
    int udp_fd = 0;
    int tcp_fd = 0;

    struct timeval timeout = {0};
    fd_set master_fds = {0};
    fd_set working_fds = {0};
    sain_t server_addr = {0};
    sain_t client_addr = {0};
    socklen_t addr_len = sizeof(client_addr);

    LoggerCreateNewFileName(logger);

    tcp_fd = CreateServer(SOCK_STREAM);
    udp_fd = CreateServer(SOCK_DGRAM);

    InitServerAddressStruct(&server_addr, atoi(port));
    BindServer(tcp_fd, &server_addr);
    BindServer(udp_fd, &server_addr);
    ListenForConnections(tcp_fd);
    InitFileDescriptorsSet(&master_fds, tcp_fd, udp_fd, STDIN_FILENO);

    max_sd = MAX2(tcp_fd, udp_fd);

    while (FALSE == end_server) /* event loop */
    {
        timeout.tv_sec = TIMEOUT_S;
        working_fds = master_fds;

        rc = select(max_sd + 1, &working_fds, NULL, NULL, &timeout);
        if (ERROR == rc)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (0 == rc) /* timeout */
        {
            LoggerInsertLine(logger
                                , "Server: Nothing happend for few seconds.");
            continue;
        }
        
        ready_fd = rc;

        for (i = 0; i <= max_sd && ready_fd > 0; ++i)
        {
            if (FD_ISSET(i, &working_fds))
            {
                --ready_fd;
                
                if (i == udp_fd)
                {
                    UDPServerCommunication(udp_fd, &client_addr, &addr_len);
                }
                
                else if (i == STDIN_FILENO)
                {
                    if (STDINCommunication())
                    {
                        end_server = TRUE;
                        break;
                    }
                }
                
                else if (i == tcp_fd) /* New connection is pending */ 
                {
                    if (NewTCPConnectionsHandler(tcp_fd, &master_fds
                                                            , &i, &max_sd))
                    {
                        end_server = TRUE;
                        break;
                    }

                    LoggerInsertLine(logger
                                    , "Server: New TCP connection established");
                }
                
                else
                {
                    TCPServerCommunication(i, &master_fds, &max_sd);
                }
            }
        }
    }

    for (i = 0; i < max_sd; ++i)
    {
        if (FD_ISSET(i, &master_fds))
        {
            close(i);
        }
    }
    
    return 0;
}

int TCPClient(const char* address, const char* port)
{
    int num_of_ping = RandInRange(3, 10);
    int tcp_fd = 0;
    int server_active = TRUE;
    sain_t server_addr = {0};

    tcp_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (0 > tcp_fd)
    {
        return ERROR;
    }

    InitServerAddressStruct(&server_addr, atoi(port));
    server_addr.sin_addr.s_addr = inet_addr(address);

    if (0 != connect(tcp_fd, (sa_t*)&server_addr, sizeof(server_addr)))
    {
        return ERROR;
    }

    LoggerInsertLine(logger, "New TCP connection.");
    
    while (num_of_ping-- && TRUE == server_active)
    {
        server_active = TCPClientCommunication(tcp_fd);
        sleep(RandInRange(3, 10));
    }

    close(tcp_fd);

    return 0;
}

int UDPClient(const char* address, const char* port)
{
    int num_of_ping = RandInRange(3, 10);
    int udp_fd = 0;
    sain_t server_addr = {0};
    sain_t client_addr = {0};

    udp_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (0 > udp_fd)
    {
        return ERROR;
    }

    InitServerAddressStruct(&server_addr, atoi(port));
    server_addr.sin_addr.s_addr = inet_addr(address);

    if (0 > connect(udp_fd, (sa_t*)&server_addr, sizeof(server_addr)))
    {
        return ERROR;
    }
    
    LoggerInsertLine(logger, "Server: New UDP connection.");
    
    while (num_of_ping--)
    {
        if (ERROR == sendto(udp_fd, PING, 5, 0, (sa_t*)&server_addr
                                                        , sizeof(server_addr)))
        {
            return ERROR;
        }

        LoggerInsertLine(logger, PING);
        
        if (ERROR == UDPClientReciveMessage(udp_fd, (sa_t*)&client_addr
                                                        , sizeof(client_addr)))
        {
            return ERROR;
        }

        sleep(RandInRange(3, 10));
    }
    
    close(udp_fd);

    return 0;    
}
