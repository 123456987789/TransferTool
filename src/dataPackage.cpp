#include "../header/dataPackage.hpp"

dataPackage::dataPackage()
{
    memset(this->PackageSegmentName, 0x00, SEGMENT_NAME_MAXLENGTH);
    memset(this->PackageSegmentData, 0x00, SEGMENT_DATA_MAXLENGTH);
    this->PackageSegmentNumber = -1;
    this->PackageSegmentEnd = false;
}

dataPackage::dataPackage(const char* _PackageSegmentName, const char* _PackageSegmentData,
    const int _PackageSegmentNumber, const int _PackageSegmentEnd, const int _PackageSegmentSize)
{
    memset(this->PackageSegmentName, 0x00, SEGMENT_NAME_MAXLENGTH);
    memset(this->PackageSegmentData, 0x00, SEGMENT_DATA_MAXLENGTH);
    memcpy(this->PackageSegmentName, _PackageSegmentName, sizeof(this->PackageSegmentName));
    memcpy(this->PackageSegmentData, _PackageSegmentData, sizeof(this->PackageSegmentData));
    this->PackageSegmentNumber = _PackageSegmentNumber;
    this->PackageSegmentEnd = _PackageSegmentEnd;
    this->PackageSegmentSize = _PackageSegmentSize;
}

dataPackage::~dataPackage()
{
}

char* dataPackage::getPackageSegmentName()
{
    return this->PackageSegmentName;
}

char* dataPackage::getPackageSegmentData()
{
    return this->PackageSegmentData;
}

void dataPackage::getPackageSegmentData(char*& pPackageSegmentData)
{
    pPackageSegmentData = PackageSegmentData;
}

int dataPackage::getPackageSegmentNumber()
{
    return this->PackageSegmentNumber;
}

int dataPackage::getPackageSegmentSize()
{
    return this->PackageSegmentSize;
}

bool dataPackage::packageSegmentisEnd()
{
    return PackageSegmentEnd == true;
}

bool dataPackage::setPackageSegmentName(const char* _PackageSegmentName)
{
    memset(this->PackageSegmentName, 0x00, SEGMENT_NAME_MAXLENGTH);
    strcpy(this->PackageSegmentName, _PackageSegmentName);
    return true;
}

bool dataPackage::setPackageSegmentData(const char* _PackageSegmentData)
{
    memset(this->PackageSegmentData, 0x00, SEGMENT_DATA_MAXLENGTH);
    strcpy(this->PackageSegmentData, _PackageSegmentData);
    return true;
}

bool dataPackage::setPackageSegmentNumber(const int _PackageSegmentNumber)
{
    this->PackageSegmentNumber = _PackageSegmentNumber;
    return true;
}

bool dataPackage::setPackageSegmentEnd()
{
    this->PackageSegmentEnd = true;
    return true;
}

bool dataPackage::operator==(dataPackage& udpack)
{
    return (strcmp(this->PackageSegmentName, udpack.getPackageSegmentName()) == 0)
        && (strcmp(this->PackageSegmentData, udpack.getPackageSegmentData()) == 0)
        && (this->PackageSegmentNumber == udpack.getPackageSegmentNumber())
        && (this->PackageSegmentSize == udpack.getPackageSegmentSize())
        && (this->packageSegmentisEnd() == udpack.packageSegmentisEnd());
}
