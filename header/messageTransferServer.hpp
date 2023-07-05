#ifndef __messageTransferServer_HPP
#define __messageTransferServer_HPP

#include "udpSocket.hpp"
#include "dataPackage.hpp"
#include "dirReader.hpp"
#include "commandAnaly.hpp"
#include "../include/cJSON.h"

#define __OLD_VERSION false
#define __WRITE_TEST false

class messageTransferServer
{
private:
    udpSocket udpServerSocket;
public:
    messageTransferServer();
    ~messageTransferServer();
    void messageTransferServerStartUp(const char* ip, const uint16_t port);
    void messageTransferServerdoGET(void* reqContext, const char* recvIP, const uint16_t& recvPORT);
    void messageTransferServerdoPUT(void* reqContext, const char* recvIP, const uint16_t& recvPORT);
    void messageTransferServerdoLIST(const char* recvIP, const uint16_t& recvPORT);
};

#endif
