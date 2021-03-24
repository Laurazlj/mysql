#ifndef _PAGE_
#define _PAGE_
#include "common.h"
#include "dbfile.h"

/*******************************************************************************
                                   Page 页
*******************************************************************************/
class Page
{
private:
    int pageSize;//page大小
    int pageID;
    bool isWirtten;//该页是否已被写
    //bool isReadLock;//是否上读锁
    //bool isWriteLock;//是否上写锁
    BYTE* pageBuffer;//用ByteBuffer模拟Cache区域
public:
    Page(int pID);//根据pageID,从datafile里拿出CByteBuffer*
    Page(int pID, BYTE* buffer);
    ~Page();
    void writeIntoFile();
    BYTE* getPageBuffer();
    int getPageID();
    bool getIsWritten();
    bool modifyPageCtx(BYTE* src, int start, int size);//修改page内容,并且setWritten
    void setWritten();
    // void readLock();
    // void readUnlock();
    // void writeLock();
    // void writeUnlock();
};

#endif