#ifndef __SERVER_H__
#define __SERVER_H__

int Server(const char* port);
int TCPClient(const char* address, const char* port);
int UDPClient(const char* address, const char* port);

#endif	/* __SERVER_H__ */
