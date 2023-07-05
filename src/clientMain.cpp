#include "../header/messageTransferClient.hpp"
#include "../header/fileTransferClient.hpp"
#include "../header/dataPackage.hpp"
#include "../header/commandAnaly.hpp"
#include <iostream>

const char* ip = "127.0.0.1";
const uint16_t msg_port = 39000;
const uint16_t fil_port = 39001;

void RunCommand()
{
    printf("Welcome File Transfer v2.0.0! [%s]\n\n", getLocalTime());
    printf("Usage:\n");
    printf(" > `get [filename]`    --download file from server.\n");
    printf(" > `put [filename]`    --upload file to server.\n");
    printf(" > `list`              --list server files.\n");
    printf(" > `exit`              --exit the sysytem.\n\n");
}

bool recvKeyboardInput(char* _usrCommand)
{
    char usrCommand[UNIFORM_BUFFER_SIZE];
    memset(usrCommand, 0x00, sizeof(usrCommand));

    printf(" > ");
    std::cin.getline(usrCommand, sizeof(usrCommand));

    if (criticalOperator(usrCommand, EXIT_SYS))
        return false;

    strcpy(_usrCommand, usrCommand);
    return true;
}

void deal1WordCommand(commandPackage* commandpackage)
{
    messageTransferClient mtc(ip, msg_port);
    mtc.Send(commandpackage);
    mtc.Recv(commandpackage);
}

void deal2WordCommand(commandPackage* commandpackage)
{
    messageTransferClient mtc(ip, msg_port);
    mtc.Send(commandpackage);
    if (mtc.Recv(commandpackage))
    {
        fileTransferClient ftc(ip, fil_port);
        ftc.ConnectToTcpServer();
        ftc.Send(commandpackage);
        if (ftc.Recv(commandpackage))
            printf("Successful!\n");
        ftc.CloseConnect();
    }
}

void RunFILETransfer()
{
    RunCommand();

    while (true)
    {
        char usrCommand[UNIFORM_BUFFER_SIZE];
        memset(usrCommand, 0x00, sizeof(usrCommand));

        if (!recvKeyboardInput(usrCommand))
            break;

        commandAnaly commandanaly(usrCommand);
        commandPackage* commandpackage = commandanaly.getcommandPackage();

        if (criticalOperator(commandpackage->reqHeader, LIST))
            deal1WordCommand(commandpackage);
        else
            deal2WordCommand(commandpackage);

        commandpackage->~commandPackage();
    }

}

int main()
{
    RunFILETransfer();
    return 0;
}
