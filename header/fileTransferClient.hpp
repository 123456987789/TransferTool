#ifndef __FILETRANSFERCLIENT_HPP
#define __FILETRANSFERCLIENT_HPP

#include "tcpSocket.hpp"

class fileTransferClient
{
private:
    tcpSocket tcpClientSocket;
    char ip[IP_SIZE];
    uint16_t port;
public:
    fileTransferClient(const char* _ip, const uint16_t& _port);
    ~fileTransferClient();
    bool ConnectToTcpServer();
    bool CloseConnect();
    bool Recv(void* recvBuffer);
    bool Send(const void* sendBuffer);
    bool fileTransferClientdoGET(char* getFileName);
    bool fileTransferClientdoPUT(char* putFileName);
};

#endif
