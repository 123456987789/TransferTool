#ifndef __TCPSOCKET_HPP
#define __TCPSOCKET_HPP

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

class tcpSocket
{
private:
    int tcpSock;
public:
    tcpSocket();
    ~tcpSocket();
    bool CreateTCPSocket();
    bool Bind2IPandPort(const char* ip, const uint16_t port);
    bool ListenSocket(const int& times);
    bool Accept(tcpSocket* peerTcpSock, char* ip, uint16_t* port);
    bool Connect(const char* ip, const uint16_t& port);
    bool RecvPackage(void* recvBuffer);
    bool SendPackage(const void* sendBuffer);
    bool CloseTCPSocket();
};

#endif
