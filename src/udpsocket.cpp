#include "../header/udpSocket.hpp"

udpSocket::udpSocket() :udpSock(-1)
{

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        printf("WSAStartup error!\n");
#else
    // Nothing todo...
#endif

}

udpSocket::~udpSocket()
{
}

bool udpSocket::CreateUDPSocket()
{
    udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (!CHECK_POINT(udpSock, "usocket"))
        return false;
    return true;
}

bool udpSocket::Bind2IPandPort(const char* ip, const uint16_t port)
{
    sockaddr_in usock_addr;
    usock_addr.sin_family = AF_INET;
    usock_addr.sin_addr.s_addr = inet_addr(ip);
    usock_addr.sin_port = htons(port);

    int bindCode = bind(udpSock, (sockaddr*)&usock_addr, sizeof(usock_addr));
    if (!CHECK_POINT(bindCode, "ubind"))
        return false;
    return true;
}

bool udpSocket::RecvPackageFrom(void* recvBuffer, void* ip = nullptr, uint16_t* port = nullptr)
{
    char tempRecvBuffer[UNIFORM_BUFFER_SIZE];
    memset(tempRecvBuffer, 0x00, sizeof(tempRecvBuffer));

    sockaddr_in net_usock_addr;
    socklen_t len = sizeof(net_usock_addr);

    int recvfromSize = recvfrom(udpSock, tempRecvBuffer, sizeof(tempRecvBuffer), 0, (sockaddr*)&net_usock_addr, &len);
    // printf("%d\n", recvfromSize);
    if (!CHECK_POINT(recvfromSize, "urecvfrom"))
        return false;

    memcpy(recvBuffer, tempRecvBuffer, recvfromSize);

    if (ip)
        memcpy(ip, inet_ntoa(net_usock_addr.sin_addr), 16);

    if (port)
        *port = ntohs(net_usock_addr.sin_port);

    return true;
}

bool udpSocket::SendPackageTo(const void* sendBuffer, const char* ip, const uint16_t port)
{
    sockaddr_in net_usock_addr;
    net_usock_addr.sin_family = AF_INET;
    net_usock_addr.sin_addr.s_addr = inet_addr(ip);
    net_usock_addr.sin_port = htons(port);

    socklen_t len = sizeof(net_usock_addr);
    int sendtoSize = sendto(udpSock, (char*)sendBuffer, UNIFORM_BUFFER_SIZE, 0, (sockaddr*)&net_usock_addr, len);
    if (!CHECK_POINT(sendtoSize, "usendto"))
        return false;
    return true;
}

bool udpSocket::CloseUDPSocket()
{
    close(udpSock);
    return true;
}
