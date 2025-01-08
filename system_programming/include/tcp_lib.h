#ifndef __TCP_LIB__
#define __TCP_LIB__

#define ERROR -1
#define NUM_OF_THREADS 10
#define BUFFER_SIZE 100

typedef struct sockaddr sa_t;
typedef struct sockaddr_in sain_t;

int TCPServer(const char* port);
int TCPClient(const char* address, const char* port);

#endif /* __TCP_LIB__ */
