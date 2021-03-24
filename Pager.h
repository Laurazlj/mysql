#pragma once
#include "common.h"
#include "PageCache.h"
#include "JournalFIle.h"
#include "dbfile.h"

/*******************************************************************************
                                   Page Manager
*******************************************************************************/
class Pager
{
private:
    string dbName;
    PageCache* pageCache;
    JournalFile journalFile;//日志文件

    int theNextUnusedPageID;//下一个没有被使用的pageID
    list<int> releasedPageID;//被释放的pageID

    void readReleasedPageIDFromFile();//从数据库文件头中得知被释放的pageID
    bool writeReleasedPageIDToFile(list<int>ReID);//把被释放的pageID写回数据库文件
    void readTheNextUnusedPageIDFromFile();
    bool writeTheNextUnusedPageIDToFile();
public:
    Pager(string dbName);
    bool isNewDatabase(string dbname);
    bool close();//关闭数据库,返回关闭是否成功
    BYTE* readPage(int pageID);//调用pageCache中的方法
    bool writePage(int pageID, BYTE* src, int pos, int size);
    int getUnusedPageID();//调用卓尼的接口,得到下一个pageID,然后将pageID++,防止该页没写回的时候,又加了一页
    bool releasePage(int pageID);
};