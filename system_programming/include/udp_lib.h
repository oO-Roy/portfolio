#ifndef __UDP_LIB__
#define __UDP_LIB__

#include <stdio.h> /*printf*/
#include <stdlib.h> /*atoi*/
#include <arpa/inet.h> /*inet_addr*/
#include <unistd.h> /*read, write*/
#include <netdb.h> /*struct sockaddr_storage, struct addrinfo, socklen_t
                                        , AF_INET, SOCK_DGRAM, AI_PASSIVE;*/

#define BUFFER_SIZE 100
#define ERROR -1

typedef struct sockaddr sa_t;
typedef struct sockaddr_in sain_t;

int UDPServer(const char* port);
int UDPClient(const char* address, const char* port);

int UDPBroadcastServer(const char* port, const char* message);
int UDPBroadcastClient(const char* port);

#endif /* __UDP_LIB__ */
