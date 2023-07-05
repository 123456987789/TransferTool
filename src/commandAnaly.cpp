#include "../header/commandAnaly.hpp"

commandAnaly::commandAnaly(char* _command)
{
    this->command = new char[UNIFORM_BUFFER_SIZE];
    memset(this->command, 0x00, UNIFORM_BUFFER_SIZE);
    strcpy(this->command, _command);
}

commandAnaly::~commandAnaly()
{
    delete[] this->command;
}

commandPackage* commandAnaly::getcommandPackage()
{
    commandPackage* cmdPack = commandPackage::getInstance();

    char* ptrcommand = this->command;

    while (*ptrcommand != '\0')
    {
        if (*ptrcommand == ' ' && strlen(cmdPack->reqHeader) == 0)
        {
            ptrcommand++;
            continue;
        }
        if (*ptrcommand == ' ')
        {
            while (*ptrcommand != '\0')
            {
                if (*ptrcommand == ' ')
                {
                    ptrcommand++;
                    continue;
                }
                sprintf(cmdPack->reqContext, "%s%c", cmdPack->reqContext, *ptrcommand);
                ptrcommand++;
            }
            break;
        }
        sprintf(cmdPack->reqHeader, "%s%c", cmdPack->reqHeader, *ptrcommand);
        ptrcommand++;
    }

    return cmdPack;
}
