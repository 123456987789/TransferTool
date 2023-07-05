#include "../header/fileTransferClient.hpp"
#include "../header/dataPackage.hpp"

fileTransferClient::fileTransferClient(const char* _ip, const uint16_t& _port) :port(_port)
{
    tcpClientSocket.CreateTCPSocket();
    memset(this->ip, 0x00, sizeof(this->ip));
    strcpy(this->ip, _ip);
}

fileTransferClient::~fileTransferClient()
{
    tcpClientSocket.CloseTCPSocket();
}

bool fileTransferClient::ConnectToTcpServer()
{
    return tcpClientSocket.Connect(this->ip, this->port);
}

bool fileTransferClient::CloseConnect()
{
    return tcpClientSocket.CloseTCPSocket();
}

bool fileTransferClient::Recv(void* recvBuffer)
{
    commandPackage* commandpackage = (commandPackage*)recvBuffer;
    if (criticalOperator(commandpackage->reqHeader, GET))
        return fileTransferClientdoGET(commandpackage->reqContext);
    else if (criticalOperator(commandpackage->reqHeader, PUT))
        return fileTransferClientdoPUT(commandpackage->reqContext);
    else
        return false;
}

bool fileTransferClient::Send(const void* sendBuffer)
{
    return tcpClientSocket.SendPackage(sendBuffer);
}

bool fileTransferClient::fileTransferClientdoGET(char* getFileName)
{
    char clientRecvBuffer[UNIFORM_BUFFER_SIZE] = { 0 };
    tcpClientSocket.RecvPackage(clientRecvBuffer);

    if (criticalOperator(clientRecvBuffer, NOT_FOUND))
        return false;

    long fileTotalLength = 0;
    sscanf(clientRecvBuffer, "%ld", &fileTotalLength);
    memset(clientRecvBuffer, 0x00, sizeof(clientRecvBuffer));

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    char filePath[SEGMENT_NAME_MAXLENGTH] = ".//filebuffer//";
#else
    char filePath[SEGMENT_NAME_MAXLENGTH] = "/root/Socket_Workspace/FILETransfer/";
#endif

    char* localhostFileName = strcat(filePath, getFileName);

    FILE* fpw;
    if ((fpw = fopen(localhostFileName, "wb")) == nullptr)
    {
        printf("fopen error!\n");
        return false;
    }

    long totalRecvLength = 0;
    int lastpos = 0;

    while (true)
    {
        tcpClientSocket.RecvPackage(clientRecvBuffer);

        if (criticalOperator(clientRecvBuffer, OK))
            break;

        tcpClientSocket.SendPackage(ACK);

        dataPackage* udpack = (dataPackage*)clientRecvBuffer;

        int fileblockLength = -1;
        if ((fileblockLength = fwrite(udpack->getPackageSegmentData(), sizeof(char), udpack->getPackageSegmentSize(), fpw)) < 0)
        {
            printf("fwrite error!\n");
            return false;
        }

        totalRecvLength += udpack->getPackageSegmentSize();
        lastpos = progress((int)(100 * 1.0 * totalRecvLength / fileTotalLength), lastpos);
        fflush(stdout);

        memset(clientRecvBuffer, 0x00, sizeof(clientRecvBuffer));
    }

    fclose(fpw);

    return true;
}

bool fileTransferClient::fileTransferClientdoPUT(char* putFileName)
{
    char tcpClientSendBuffer[UNIFORM_BUFFER_SIZE] = { 0 };

    FILE* fpr;
    if ((fpr = fopen(putFileName, "rb")) == nullptr)
    {
        printf("fopen error!\n");
        return false;
    }

    long fileTotalLength = obtainFileLength(fpr);
    fseek(fpr, 0L, SEEK_SET);

    tcpClientSocket.SendPackage(LongIntToString(fileTotalLength));
    tcpClientSocket.RecvPackage(tcpClientSendBuffer);
    if (!criticalOperator(tcpClientSendBuffer, ACK))
        return false;

    memset(tcpClientSendBuffer, 0x00, sizeof(tcpClientSendBuffer));

    int fileblockLength = -1;
    int segmentNumber = 1;

    int readsize = SEGMENT_DATA_MAXLENGTH - 2;

    long totalSendLength = 0;
    long totalLength = fileTotalLength;
    int lastpos = 0;

    printf("Uploading......\n");

    while (fileTotalLength)
    {
        if (fileTotalLength < readsize)
        {
            fileblockLength = fread(tcpClientSendBuffer, sizeof(char), fileTotalLength, fpr);
            fileblockLength = fileTotalLength;
            fileTotalLength = 0;
        }
        else
        {
            fileblockLength = fread(tcpClientSendBuffer, sizeof(char), readsize, fpr);
            fileblockLength = readsize;
            fileTotalLength -= readsize;
        }

        char segmentName[UNIFORM_BUFFER_SIZE] = { 0 };
        sprintf(segmentName, "tcp/pack/%s/segment/%d/%d", resolutionFileName_FromFilePath(putFileName), segmentNumber, 0);

        dataPackage udpack(segmentName, tcpClientSendBuffer, segmentNumber, false, fileblockLength);
        tcpClientSocket.SendPackage(&udpack);

        char clientRecvACK[UNIFORM_BUFFER_SIZE] = { 0 };
        tcpClientSocket.RecvPackage(clientRecvACK);
        if (!criticalOperator(clientRecvACK, ACK))
            break;

        // totalSendLength += udpack.getPackageSegmentSize();
        // // printf("Send Peocess : %f/%\n", (100 * 1.0 * totalSendLength / totalLength));
        // lastpos = progress((int)(100 * 1.0 * totalSendLength / totalLength), lastpos);
        // fflush(stdout);
        // printf("%s, %d\n", segmentName, udpack.getPackageSegmentSize());

        memset(tcpClientSendBuffer, 0x00, sizeof(tcpClientSendBuffer));
        segmentNumber++;
    }

    fclose(fpr);

    memset(tcpClientSendBuffer, 0x00, sizeof(tcpClientSendBuffer));

    tcpClientSocket.SendPackage(OK);
    return true;
}
