#ifndef __UDPSOCKET_HPP
#define __UDPSOCKET_HPP

#include "utils.hpp"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
#include <winsock2.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

class udpSocket
{
private:
    int udpSock;
public:
    udpSocket();
    ~udpSocket();
    bool CreateUDPSocket();
    bool Bind2IPandPort(const char* ip, const uint16_t port);
    bool RecvPackageFrom(void* recvBuffer, void* ip, uint16_t* port);
    bool SendPackageTo(const void* sendBuffer, const char* ip, const uint16_t port);
    bool CloseUDPSocket();
};

#endif
