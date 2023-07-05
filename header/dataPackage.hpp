#ifndef __dataPackage_HPP
#define __dataPackage_HPP

#include "utils.hpp"
#include <string.h>

class dataPackage
{
private:
    char PackageSegmentName[SEGMENT_NAME_MAXLENGTH];
    char PackageSegmentData[SEGMENT_DATA_MAXLENGTH];
    int PackageSegmentNumber;
    int PackageSegmentSize;
    bool PackageSegmentEnd;
public:
    dataPackage();
    dataPackage(const char* _PackageSegmentName, const char* _PackageSegmentData,
        const int _PackageSegmentNumber, const int _PackageSegmentEnd, const int _PackageSegmentSize);
    ~dataPackage();
    char* getPackageSegmentName();
    char* getPackageSegmentData();
    void getPackageSegmentData(char*& pPackageSegmentData);
    int getPackageSegmentNumber();
    int getPackageSegmentSize();
    bool packageSegmentisEnd();
    bool setPackageSegmentName(const char* _PackageSegmentName);
    bool setPackageSegmentData(const char* _PackageSegmentData);
    bool setPackageSegmentNumber(const int _PackageSegmentNumber);
    bool setPackageSegmentEnd();
    bool operator==(dataPackage& udpack);
    void getdata(char* buf)
    {
        memcpy(buf, PackageSegmentData, PackageSegmentSize);
    }
};

#endif
