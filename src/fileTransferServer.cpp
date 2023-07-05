#include "../header/fileTransferServer.hpp"

fileTransferServer::fileTransferServer(const char* _ip, const uint16_t& _port) :port(_port)
{
    memset(this->ip, 0x00, sizeof(this->ip));
    strcpy(this->ip, _ip);
}

fileTransferServer::~fileTransferServer()
{
}

void fileTransferServer::fileTransferServerStartUp()
{
    tcpServerSocket.CreateTCPSocket();
    tcpServerSocket.Bind2IPandPort(this->ip, this->port);
    tcpServerSocket.ListenSocket(LISTEN_TIMES);

    printf("[%s] TCP File Transfer Server Start Up...\n", getLocalTime());

    while (true)
    {
        tcpSocket peerTcpSocket;
        char peerIP[IP_SIZE] = { 0 };
        uint16_t peerPORT = 0;

        if (!tcpServerSocket.Accept(&peerTcpSocket, peerIP, &peerPORT))
            continue;

        printf("[%s] HOST [%s:%d] connected...\n", getLocalTime(), peerIP, peerPORT);

        while (true)
        {
            char buf[UNIFORM_BUFFER_SIZE] = { 0 };

            if (!peerTcpSocket.RecvPackage(buf))
            {
                printf("[%s] HOST [%s:%d] disconnected...\n", getLocalTime(), peerIP, peerPORT);
                peerTcpSocket.CloseTCPSocket();
                break;
            }

            commandPackage* commandpackage = (commandPackage*)buf;

            if (criticalOperator(commandpackage->reqHeader, GET))
                fileTransferServerdoGET(commandpackage->reqContext, peerTcpSocket, peerIP, peerPORT);
            else if (criticalOperator(commandpackage->reqHeader, PUT))
                fileTransferServerdoPUT(commandpackage->reqContext, peerTcpSocket, peerIP, peerPORT);
            else
                continue;

            commandpackage->~commandPackage();
        }
    }

    tcpServerSocket.CloseTCPSocket();
}

void fileTransferServer::fileTransferServerdoGET(
    char* reqContext, tcpSocket& peerTcpSocket, const char* peerIP, const uint16_t& peerPORT)
{
    char tcpServerSendBuffer[UNIFORM_BUFFER_SIZE] = { 0 };
    char filePath[SEGMENT_NAME_MAXLENGTH * 2] = SERVER_ROOT_PATH;
    char* fileName = strcat(filePath, (char*)reqContext);

    FILE* fpr;
    if ((fpr = fopen(fileName, "rb")) == nullptr)
        peerTcpSocket.SendPackage(NOT_FOUND);

    long fileTotalLength = obtainFileLength(fpr);
    fseek(fpr, 0L, SEEK_SET);

    peerTcpSocket.SendPackage(LongIntToString(fileTotalLength));

    memset(tcpServerSendBuffer, 0x00, sizeof(tcpServerSendBuffer));

    int fileblockLength = -1;
    int segmentNumber = 1;

    int readsize = SEGMENT_DATA_MAXLENGTH - 2;

    long totalSendLength = 0;
    // long totalLength = fileTotalLength;
    // int lastpos = 0;

    printf("[%s] HOST [%s:%d] Sending......\n", getLocalTime(), peerIP, peerPORT);

    while (fileTotalLength)
    {
        if (fileTotalLength < readsize)
        {
            fileblockLength = fread(tcpServerSendBuffer, sizeof(char), fileTotalLength, fpr);
            fileblockLength = fileTotalLength;
            fileTotalLength = 0;
        }
        else
        {
            fileblockLength = fread(tcpServerSendBuffer, sizeof(char), readsize, fpr);
            fileblockLength = readsize;
            fileTotalLength -= readsize;
        }

        char segmentName[UNIFORM_BUFFER_SIZE] = { 0 };
        sprintf(segmentName, "tcp/pack/%s/segment/%d/%d", reqContext, segmentNumber, 0);

        dataPackage udpack(segmentName, tcpServerSendBuffer, segmentNumber, false, fileblockLength);
        peerTcpSocket.SendPackage(&udpack);

        char clientRecvACK[UNIFORM_BUFFER_SIZE] = { 0 };
        peerTcpSocket.RecvPackage(clientRecvACK);
        if (!criticalOperator(clientRecvACK, ACK))
            break;

        // totalSendLength += udpack.getPackageSegmentSize();
        // // printf("Send Peocess : %f/%\n", (100 * 1.0 * totalSendLength / totalLength));
        // lastpos = progress((int)(100 * 1.0 * totalSendLength / totalLength), lastpos);
        // fflush(stdout);
        // // printf("%s, %d\n", segmentName, udpack.getPackageSegmentSize());

        memset(tcpServerSendBuffer, 0x00, sizeof(tcpServerSendBuffer));
        segmentNumber++;
    }

    fclose(fpr);

    memset(tcpServerSendBuffer, 0x00, sizeof(tcpServerSendBuffer));

    peerTcpSocket.SendPackage(OK);
    printf("[%s] HOST [%s:%d] Sended Successful......\n", getLocalTime(), peerIP, peerPORT);

}
void fileTransferServer::fileTransferServerdoPUT(
    char* reqContext, tcpSocket& peerTcpSocket, const char* peerIP, const uint16_t& peerPORT)
{
    char tcpServerRecvBuffer[UNIFORM_BUFFER_SIZE] = { 0 };
    char filePath[SEGMENT_NAME_MAXLENGTH] = SERVER_ROOT_PATH;
    char* reqfileName = resolutionFileName_FromFilePath(reqContext);
    char* fileName = strcat(filePath, reqfileName);

    peerTcpSocket.RecvPackage(tcpServerRecvBuffer);
    peerTcpSocket.SendPackage(ACK);

    long fileTotalLength = 0;
    sscanf(tcpServerRecvBuffer, "%ld", &fileTotalLength);
    memset(tcpServerRecvBuffer, 0x00, sizeof(tcpServerRecvBuffer));

    // #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    //     char filePath[SEGMENT_NAME_MAXLENGTH] = ".//filebuffer//";
    // #else
    //     char filePath[SEGMENT_NAME_MAXLENGTH] = "../filebuffer/recv/";
    // #endif

    FILE* fpw;
    if ((fpw = fopen(fileName, "wb")) == nullptr)
    {
        printf("fopen error!\n");
    }

    long totalRecvLength = 0;
    int lastpos = 0;

    while (true)
    {
        peerTcpSocket.RecvPackage(tcpServerRecvBuffer);

        if (criticalOperator(tcpServerRecvBuffer, OK))
            break;

        dataPackage* udpack = (dataPackage*)tcpServerRecvBuffer;

        int fileblockLength = -1;
        if ((fileblockLength = fwrite(udpack->getPackageSegmentData(), sizeof(char), udpack->getPackageSegmentSize(), fpw)) <= 0)
        {
            printf("fwrite error!\n");
            break;
        }

        peerTcpSocket.SendPackage(ACK);

        // totalRecvLength += udpack->getPackageSegmentSize();
        // lastpos = progress((int)(100 * 1.0 * totalRecvLength / fileTotalLength), lastpos);
        // fflush(stdout);
        // printf("%s, %d\n", udpack->getPackageSegmentName(), udpack->getPackageSegmentSize());

        memset(tcpServerRecvBuffer, 0x00, sizeof(tcpServerRecvBuffer));
    }

    fclose(fpw);
    printf("[%s] HOST [%s:%d] Recved Successful......\n", getLocalTime(), peerIP, peerPORT);
}
