#ifndef __messageTransferClient_HPP
#define __messageTransferClient_HPP

#include "udpSocket.hpp"
#include "dataPackage.hpp"
#include "dirReader.hpp"
#include "../include/cJSON.h"

#define __OLD_VERSION true

class messageTransferClient
{
private:
    char ip[IP_SIZE];
    uint16_t port;
    udpSocket udpClientSocket;
public:
    messageTransferClient(const char* _ip, const uint16_t _port);
    ~messageTransferClient();
    void Send(const void* sendBuffer);
    bool Recv(void* recvBuffer);
    bool messageTransferClientdoGET();
    bool messageTransferClientdoPUT();
    bool messageTransferClientdoLIST();
};

#endif
