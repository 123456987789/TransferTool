#ifndef __UTILS_HPP
#define __UTILS_HPP

#include <time.h>
#include <stdio.h>
#include <string.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
typedef unsigned short uint16_t;
typedef int socklen_t;
#endif

#define NOT_FOUND "not found"
#define EXIT_SYS "exit"
#define ACK "ACK"
#define OK "OK"
#define GET "get"
#define PUT "put"
#define LIST "list"
#define SERVER_ROOT_PATH "E:\\github\\TransferTool\\filebuffer"

#define IP_SIZE 32
#define LISTEN_TIMES 5
#define COMMAND_REQ_HEADER_MAXLENGTH 64
#define SEGMENT_NAME_MAXLENGTH 64
#define SEGMENT_DATA_MAXLENGTH 1300
#define UNIFORM_BUFFER_SIZE 1400

struct commandPackage
{
    char reqHeader[COMMAND_REQ_HEADER_MAXLENGTH] = {0};
    char reqContext[SEGMENT_NAME_MAXLENGTH] = {0};

    commandPackage()
    {
        memset(reqHeader, 0x00, sizeof(reqHeader));
        memset(reqContext, 0x00, sizeof(reqContext));
    }

    void setcommandPackage(const char *_reqHeader, const char *_reqContext)
    {
        strcpy(this->reqHeader, _reqHeader);
        strcpy(this->reqContext, _reqContext);
    }

    static commandPackage *getInstance()
    {
        static commandPackage commandpackage;
        return &commandpackage;
    }

    ~commandPackage()
    {
        reqHeader[COMMAND_REQ_HEADER_MAXLENGTH] = {0};
        reqContext[SEGMENT_NAME_MAXLENGTH] = {0};
        memset(reqHeader, 0x00, sizeof(reqHeader));
        memset(reqContext, 0x00, sizeof(reqContext));
    }
};

struct HEADER_GET_PUT_Package
{
    bool is_exist = false;
    long length = 0;
    char file_name[SEGMENT_NAME_MAXLENGTH] = {0};

    HEADER_GET_PUT_Package()
    {
        memset(file_name, 0x00, sizeof(file_name));
    }

    void setHEADER_GET_PUT_Package(const bool &_is_exist, const long &_length, const char *_file_name)
    {
        this->is_exist = _is_exist;
        this->length = _length;
        strcpy(this->file_name, _file_name);
    }

    static HEADER_GET_PUT_Package *getInstance()
    {
        static HEADER_GET_PUT_Package header_gpp_Instance;
        return &header_gpp_Instance;
    }

    ~HEADER_GET_PUT_Package()
    {
        is_exist = false;
        length = 0;
        memset(file_name, 0x00, sizeof(file_name));
    }
};

static char *resolutionFileName_FromFilePath(char *filePath)
{
    char *fileName = new char[COMMAND_REQ_HEADER_MAXLENGTH];
    memset(fileName, 0x00, COMMAND_REQ_HEADER_MAXLENGTH);

    int index = 0;
    char *ptrFilePath = filePath;
    while ((*ptrFilePath) != '\0')
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        ;
#else
        if ((*ptrFilePath) == '/' || (*ptrFilePath) == '\\')
        {
            memset(fileName, 0x00, COMMAND_REQ_HEADER_MAXLENGTH);
            index = 0;
        }
        else
        {
            fileName[index++] = *ptrFilePath;
        }
#endif
        ptrFilePath++;
    }
    return fileName;
}

static char *IntToString(const int &ival)
{
    char *itos = new char[16];
    memset(itos, 0x00, 16);
    sprintf(itos, "%d", ival);
    return itos;
}

static char *LongIntToString(const long &ival)
{
    char *litos = new char[16];
    memset(litos, 0x00, 16);
    sprintf(litos, "%ld", ival);
    return litos;
}

static long obtainFileLength(FILE *fp_FILE)
{
    fseek(fp_FILE, 0L, SEEK_END);
    return ftell(fp_FILE);
}

static bool criticalOperator(const char *_first_operator, const char *_second_operator)
{
    return (strcmp(_first_operator, _second_operator) == 0);
}

static bool CHECK_POINT(const int number, const char *point)
{
    if (number < 0)
    {
        printf("%s error! exit code %d\n", point, number);
        return false;
    }
    return true;
}

static int progress(int prog, int lastpos)
{
    int i = 0;
    for (i = 0; i < lastpos; i++)
    {
        printf("\b");
    }
    for (i = 0; i < prog; i++)
    {
        printf("#");
    }
    printf(">>");
    for (i += 2; i < 104; i++)
    {
        printf(" ");
    }
    i = i + printf("[%d%%]", prog);
    fflush(stdout);
    return i;
}

static char *getLocalTime()
{
    time_t timep;
    time(&timep);
    static char localTime[256];
    strftime(localTime, sizeof(localTime), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    return localTime;
}

#endif
