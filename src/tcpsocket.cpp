#include "../header/tcpSocket.hpp"

tcpSocket::tcpSocket() :tcpSock(-1)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        printf("WSAStartup error!\n");
#else
    // Nothing todo...
#endif
}

tcpSocket::~tcpSocket()
{
}

bool tcpSocket::CreateTCPSocket()
{
    tcpSock = socket(AF_INET, SOCK_STREAM, 0);
    if (!CHECK_POINT(tcpSock, "tsocket"))
        return false;
    return true;
}

bool tcpSocket::Bind2IPandPort(const char* ip, const uint16_t port)
{
    sockaddr_in tsock_addr;
    tsock_addr.sin_family = AF_INET;
    tsock_addr.sin_addr.s_addr = inet_addr(ip);
    tsock_addr.sin_port = htons(port);

    int bindCode = bind(tcpSock, (sockaddr*)&tsock_addr, sizeof(tsock_addr));
    if (!CHECK_POINT(bindCode, "tbind"))
        return false;
    return true;
}

bool tcpSocket::ListenSocket(const int& times)
{
    int listenCode = listen(tcpSock, times);
    if (!CHECK_POINT(listenCode, "tlisten"))
        return false;
    return true;
}

bool tcpSocket::Accept(tcpSocket* peerTcpSock, char* ip = nullptr, uint16_t* port = nullptr)
{
    sockaddr_in net_tsock_addr;
    socklen_t len = sizeof(net_tsock_addr);

    int peertcpSock = accept(tcpSock, (sockaddr*)&net_tsock_addr, &len);
    if (!CHECK_POINT(peertcpSock, "taccept"))
        return false;

    // memcpy(peerTcpSock, &peertcpSock, sizeof(int));
    peerTcpSock->tcpSock = peertcpSock;

    if (ip)
        memcpy(ip, inet_ntoa(net_tsock_addr.sin_addr), IP_SIZE);

    if (port)
        *port = ntohs(net_tsock_addr.sin_port);

    return true;
}

bool tcpSocket::Connect(const char* ip, const uint16_t& port)
{
    sockaddr_in tsock_addr;
    tsock_addr.sin_family = AF_INET;
    tsock_addr.sin_addr.s_addr = inet_addr(ip);
    tsock_addr.sin_port = htons(port);

    int connectCode = connect(tcpSock, (sockaddr*)&tsock_addr, sizeof(tsock_addr));
    if (!CHECK_POINT(connectCode, "tconnect"))
        return false;
    return true;
}

bool tcpSocket::RecvPackage(void* recvBuffer = nullptr)
{
    char tempRecvBuffer[UNIFORM_BUFFER_SIZE];
    memset(tempRecvBuffer, 0x00, sizeof(tempRecvBuffer));

    int recvsize = recv(tcpSock, tempRecvBuffer, sizeof(tempRecvBuffer), 0);
    if (!CHECK_POINT(recvsize, "trecv"))
        return false;

    if (!recvsize)
        return false;

    if (recvBuffer)
        memcpy(recvBuffer, tempRecvBuffer, sizeof(tempRecvBuffer));
    return true;
}

bool tcpSocket::SendPackage(const void* sendBuffer)
{
    int sendsize = send(tcpSock, (char*)sendBuffer, UNIFORM_BUFFER_SIZE, 0);
    if (!CHECK_POINT(sendsize, "tsend"))
        return false;
    return true;
}

bool tcpSocket::CloseTCPSocket()
{
    return (shutdown(tcpSock, 2) == 0);
}

