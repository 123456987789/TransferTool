#include "../header/dirReader.hpp"

dirReader::dirReader(char* _dirPath)
{
    this->numbers = 0;
    this->dirPath = new char[SEGMENT_NAME_MAXLENGTH * 2];
    this->fileContainer_cjson = cJSON_CreateObject();
    memset(this->dirPath, 0x00, SEGMENT_NAME_MAXLENGTH * 2);
    strcpy(this->dirPath, _dirPath);
}

dirReader::~dirReader()
{
}

void dirReader::readDirPath()
{
    struct dirent* ptrFile;
    DIR* ptrDirPath;

    if ((ptrDirPath = opendir(this->dirPath)) == nullptr)
    {
        printf("dir open error!\n%s\n", this->dirPath);
        return;
    }

    while ((ptrFile = readdir(ptrDirPath)) != nullptr)
    {
        if (strcmp(ptrFile->d_name, ".") == 0 || strcmp(ptrFile->d_name, "..") == 0)
            continue;
        else
            cJSON_AddStringToObject(this->fileContainer_cjson, IntToString(++(this->numbers)), ptrFile->d_name);
    }
}

cJSON* dirReader::getfileContainer_cjson()
{
    return this->fileContainer_cjson;
}
