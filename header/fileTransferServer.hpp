#ifndef __FILETRANSFERSERVER_HPP
#define __FILETRANSFERSERVER_HPP

#include "tcpSocket.hpp"
#include "dataPackage.hpp"
#include "commandAnaly.hpp"

class fileTransferServer
{
private:
    tcpSocket tcpServerSocket;
    char ip[IP_SIZE];
    uint16_t port;
public:
    fileTransferServer(const char* _ip, const uint16_t& _port);
    ~fileTransferServer();
    void fileTransferServerStartUp();
    void fileTransferServerdoGET(char* reqContext, tcpSocket& peerTcpSocket, const char* peerIP, const uint16_t& peerPORT);
    void fileTransferServerdoPUT(char* reqContext, tcpSocket& peerTcpSocket, const char* peerIP, const uint16_t& peerPORT);
};

#endif
