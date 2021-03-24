#include "Page.h"

Page::Page(int pID):
    pageSize(PAGESIZE), pageID(pID), isWirtten(false)
{
    pageBuffer = new BYTE[pageSize];
    pageBuffer=FindPageFromFile(pageID);
    //@TODO: 调用卓尼的接口,填充pageBuffer中的内容
}

Page::Page(int pID, BYTE* buffer):
    pageSize(PAGESIZE), pageID(pID), isWirtten(false), pageBuffer(buffer)
{
}

Page::~Page()
{
    delete pageBuffer;
}

int Page::getPageID()
{
    return pageID;
}

bool Page::getIsWritten()
{
    return isWirtten;
}

void Page::writeIntoFile()
{
    cout<<"-----------------------"<<endl;
    cout<<"pageBuffer: "<<pageBuffer<<endl;
    WriteBack(pageID, pageBuffer);
    BYTE * tmp = FindPageFromFile(pageID);
    cout<<"tmp: "<<tmp<<endl;
    cout<<"-----------------------"<<endl;
}

BYTE* Page::getPageBuffer()
{
    return pageBuffer;
}

void Page::setWritten()
{
    isWirtten = true;
}

bool Page::modifyPageCtx(BYTE* src, int start, int size)
{
    memcpy(pageBuffer+start, src, size);
    return true;
}