#include <stdio.h> /*printf*/
#include <stdlib.h> /*atoi*/
#include <string.h> /*memset*/
#include <pthread.h> /*pthread_create, pthread_join*/
#include <arpa/inet.h> /*inet_addr*/
#include <unistd.h> /*read, write*/
#include <fcntl.h> /*fcntl, F_GETFL, F_SETFL, O_NONBLOCK*/
#include <netinet/in.h> /*socket, AF_INET, SOCK_DGRAM
                                , struct sockaddr_in, INADDR_ANY, */

#include "tcp_lib.h"

static int CreateTCPSocket(sain_t* server_address, const uint16_t port)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (0 > socket_fd)
    {
        return ERROR;
    }

    server_address->sin_family = AF_INET;
    server_address->sin_port = htons(port);
    server_address->sin_addr.s_addr = INADDR_ANY;

    return socket_fd;
}

static void* ServerCommunication(void* connect_fd)
{
    char buffer[BUFFER_SIZE] = {0};

    while (1)
    {
        recv(*(int*)connect_fd, buffer, BUFFER_SIZE, 0);
        printf("%s\n", buffer);
        send(*(int*)connect_fd, "pong", 5, 0);
        sleep(1);
    }

    return NULL;
}

static void ClientCommunication(const int connect_fd)
{
    char buffer[BUFFER_SIZE] = {0};
    
    while (1)
    {
        send(connect_fd, "ping", 5, 0);
        recv(connect_fd, buffer, BUFFER_SIZE, 0);
        printf("%s\n", buffer);
        sleep(1);
    }
}

/******************************************************************************/
int TCPServer(const char* port)
{
    int i = 0;
    int tcp_fd = 0;
    int connect_fd = 0;
    sain_t server_addr = {0};
    sain_t client_addr = {0};
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id[NUM_OF_THREADS] = {0};
    int thread_args[NUM_OF_THREADS] = {0};

    tcp_fd = CreateTCPSocket(&server_addr, atoi(port));
    if (ERROR == tcp_fd)
    {
        return ERROR;
    }

    if (ERROR == bind(tcp_fd, (sa_t*)&server_addr, sizeof(server_addr)))
    {
        return ERROR;
    }
    
    if (ERROR == listen(tcp_fd, NUM_OF_THREADS))
    {
        return ERROR;
    }

    for (i = 0; i < NUM_OF_THREADS; ++i) /*this part is new - testing threads */
    {
        connect_fd = accept(tcp_fd, (sa_t*)&client_addr, &client_len);
        if (ERROR == connect_fd)
        {
            return ERROR;
        }

        thread_args[i] = connect_fd;
        pthread_create(&thread_id[i], NULL, &ServerCommunication
                                                            , &thread_args[i]);
    }

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        pthread_join(thread_id[i], NULL);
    }

    close(tcp_fd);

    return 0;
}

int TCPClient(const char* address, const char* port)
{
    int tcp_fd = 0;
    sain_t server_addr = {0};
    
    tcp_fd = CreateTCPSocket(&server_addr, atoi(port));
    if (ERROR == tcp_fd)
    {
        return ERROR;
    }

    server_addr.sin_addr.s_addr = inet_addr(address);

    if (0 != connect(tcp_fd, (sa_t*)&server_addr, sizeof(server_addr)))
    {
        return ERROR;
    }
    
    ClientCommunication(tcp_fd);

    close(tcp_fd);

    return 0;
}

