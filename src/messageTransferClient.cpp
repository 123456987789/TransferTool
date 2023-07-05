#include "../header/messageTransferClient.hpp"

messageTransferClient::messageTransferClient(const char* _ip, const uint16_t _port) :port(_port)
{
    memset(ip, 0x00, sizeof(ip));
    strcpy(ip, _ip);

    udpClientSocket.CreateUDPSocket();
}

messageTransferClient::~messageTransferClient()
{
    udpClientSocket.CloseUDPSocket();
}

void messageTransferClient::Send(const void* sendBuffer)
{
    udpClientSocket.SendPackageTo(sendBuffer, this->ip, this->port);
}

bool messageTransferClient::Recv(void* recvBuffer)
{
    commandPackage* commandpackage = (commandPackage*)recvBuffer;
    if (criticalOperator(commandpackage->reqHeader, GET))
        return messageTransferClientdoGET();
    else if (criticalOperator(commandpackage->reqHeader, PUT))
    {
        FILE* fpr;
        if ((fpr = fopen(commandpackage->reqContext, "rb")) == nullptr)
        {
            printf("Path error! Not found this file in your localhost!\n");
            return false;
        }
        fclose(fpr);
        return messageTransferClientdoPUT();
    }
    else if (criticalOperator(commandpackage->reqHeader, LIST))
        return messageTransferClientdoLIST();
    else
        return false;
}

bool messageTransferClient::messageTransferClientdoGET()
{
    HEADER_GET_PUT_Package* header_gpp = HEADER_GET_PUT_Package::getInstance();

    udpClientSocket.RecvPackageFrom(header_gpp, nullptr, nullptr);
    if (!header_gpp->is_exist)
    {
        printf("Not Found!\n");
        header_gpp->~HEADER_GET_PUT_Package();
        return false;
    }

    printf("file [%s] Total Lendth : [%ld] bits\n", header_gpp->file_name, header_gpp->length);
    header_gpp->~HEADER_GET_PUT_Package();
    return true;
}

bool messageTransferClient::messageTransferClientdoPUT()
{
    HEADER_GET_PUT_Package* header_gpp = HEADER_GET_PUT_Package::getInstance();

    udpClientSocket.RecvPackageFrom(header_gpp, nullptr, nullptr);
    if (!header_gpp->is_exist)
    {
        printf("Allowed upload!\n");
        header_gpp->~HEADER_GET_PUT_Package();
        return true;
    }

    printf("file [%s] also exists, Total Lendth : [%ld] bits, can't allowed upload!\n", header_gpp->file_name, header_gpp->length);
    header_gpp->~HEADER_GET_PUT_Package();
    return false;
}

bool messageTransferClient::messageTransferClientdoLIST()
{
    char* dirFileList = new char[UNIFORM_BUFFER_SIZE];
    memset(dirFileList, 0x00, UNIFORM_BUFFER_SIZE);

    udpClientSocket.RecvPackageFrom(dirFileList, nullptr, nullptr);

    cJSON* dirFileList_cJSON = cJSON_Parse(dirFileList);
    if (!dirFileList_cJSON)
    {
        printf("cJSON error!%s\n", cJSON_GetErrorPtr());
        return false;
    }

    cJSON* getDirFileListItem_cJSON;
    for (int itemIndex = 1; ; itemIndex++)
    {
        getDirFileListItem_cJSON = cJSON_GetObjectItem(dirFileList_cJSON, IntToString(itemIndex));

        if (!getDirFileListItem_cJSON)
            break;

        if (getDirFileListItem_cJSON->type == cJSON_String)
            printf("%s   ", getDirFileListItem_cJSON->valuestring);
    }
    printf("\n");

    return true;
}
