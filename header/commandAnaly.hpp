#ifndef __COMMANDANALY_HPP
#define __COMMANDANALY_HPP

#include "utils.hpp"
#include <string.h>

typedef struct commandPackage commandPackage;

class commandAnaly
{
private:
    char* command;
public:
    commandAnaly(char* _command);
    ~commandAnaly();
    commandPackage* getcommandPackage();
};

#endif
