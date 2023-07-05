#include "../header/messageTransferServer.hpp"

messageTransferServer::messageTransferServer()
{
    udpServerSocket.CreateUDPSocket();
}

messageTransferServer::~messageTransferServer()
{
    udpServerSocket.CloseUDPSocket();
}

void messageTransferServer::messageTransferServerStartUp(const char* ip, const uint16_t port)
{
    udpServerSocket.Bind2IPandPort(ip, port);
    printf("[%s] UDP File Transfer Server Start Up...\n", getLocalTime());

    while (true)
    {
        char recvIP[IP_SIZE] = { 0 };
        uint16_t recvPORT = 0;

        char buf[UNIFORM_BUFFER_SIZE] = { 0 };

        if (!udpServerSocket.RecvPackageFrom(buf, recvIP, &recvPORT))
            continue;

        commandPackage* commandpackage = (commandPackage*)buf;

        if (criticalOperator(commandpackage->reqHeader, GET))
            messageTransferServerdoGET(commandpackage->reqContext, recvIP, recvPORT);   //get
        else if (criticalOperator(commandpackage->reqHeader, PUT))
            messageTransferServerdoPUT(commandpackage->reqContext, recvIP, recvPORT);   //put
        else if (criticalOperator(commandpackage->reqHeader, LIST))
            messageTransferServerdoLIST(recvIP, recvPORT);   //list           
        else
            continue;   //nothing todo

        commandpackage->~commandPackage();
    }
}

void messageTransferServer::messageTransferServerdoGET(void* reqContext, const char* recvIP, const uint16_t& recvPORT)
{
    char filePath[SEGMENT_NAME_MAXLENGTH * 2] = SERVER_ROOT_PATH;
    char* fileName = strcat(filePath, (char*)reqContext);

    HEADER_GET_PUT_Package* header_gpp = HEADER_GET_PUT_Package::getInstance();

    FILE* fpr;
    if ((fpr = fopen(fileName, "rb")) == nullptr)
    {
        udpServerSocket.SendPackageTo(header_gpp, recvIP, recvPORT);
        printf("[%s] HOST [%s:%d] REQ : %s NOT FOUND!, RESP : %ld bits\n", getLocalTime(), recvIP, recvPORT, fileName, header_gpp->length);
        header_gpp->~HEADER_GET_PUT_Package();
        return;
    }

    long fileTotalLength = obtainFileLength(fpr);
    fclose(fpr);

    header_gpp->setHEADER_GET_PUT_Package(true, fileTotalLength, fileName);

    udpServerSocket.SendPackageTo(header_gpp, recvIP, recvPORT);
    printf("[%s] HOST [%s:%d] REQ : %s, RESP : %ld bits\n", getLocalTime(), recvIP, recvPORT, fileName, header_gpp->length);
    header_gpp->~HEADER_GET_PUT_Package();
}

void messageTransferServer::messageTransferServerdoPUT(void* reqContext, const char* recvIP, const uint16_t& recvPORT)
{
    char filePath[SEGMENT_NAME_MAXLENGTH * 2] = SERVER_ROOT_PATH;
    char* reqfileName = resolutionFileName_FromFilePath((char*)reqContext);
    char* fileName = strcat(filePath, reqfileName);

    HEADER_GET_PUT_Package* header_gpp = HEADER_GET_PUT_Package::getInstance();

    FILE* fpr;
    if ((fpr = fopen(fileName, "rb")) == nullptr)
    {
        udpServerSocket.SendPackageTo(header_gpp, recvIP, recvPORT);
        printf("[%s] HOST [%s:%d] REQ : %s NOT EXISTS THE SAME FILE, ALLOWED UPLOAD!, RESP : %ld bits\n", getLocalTime(), recvIP, recvPORT, fileName, header_gpp->length);
        header_gpp->~HEADER_GET_PUT_Package();
        return;
    }

    long fileTotalLength = obtainFileLength(fpr);
    fclose(fpr);

    header_gpp->setHEADER_GET_PUT_Package(true, fileTotalLength, fileName);

    udpServerSocket.SendPackageTo(header_gpp, recvIP, recvPORT);
    printf("[%s] HOST [%s:%d] REQ : %s EXISTS THE SAME FILE, CAN'T ALLOWED UPLOAD!, RESP : %ld bits\n", getLocalTime(), recvIP, recvPORT, fileName, header_gpp->length);
    header_gpp->~HEADER_GET_PUT_Package();
}

void messageTransferServer::messageTransferServerdoLIST(const char* recvIP, const uint16_t& recvPORT)
{
    dirReader* dirFileReader = new dirReader((char*)SERVER_ROOT_PATH);
    dirFileReader->readDirPath();

    cJSON* dirFileContainer = dirFileReader->getfileContainer_cjson();

    char* sendbuf = cJSON_Print(dirFileContainer);
    udpServerSocket.SendPackageTo(sendbuf, recvIP, recvPORT);

    printf("[%s] HOST [%s:%d] REQ : %s, RESP : As follows\n", getLocalTime(), recvIP, recvPORT, LIST);
    printf("%s\n", sendbuf);
}
