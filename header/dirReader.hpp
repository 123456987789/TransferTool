#ifndef __DIRREADER_HPP
#define __DIRREADER_HPP

#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include "utils.hpp"
#include "../include/cJSON.h"

class dirReader
{
private:
    int numbers;
    char* dirPath;
    cJSON* fileContainer_cjson;
public:
    dirReader(char* _dirPath);
    ~dirReader();
    void readDirPath();
    cJSON* getfileContainer_cjson();
};

#endif
