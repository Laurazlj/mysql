#include "common.h"
#include "Page.h"
#include "dbfile.h"

class PageCache
{
private:
    string dbName;

    int pageSize;//页大小
    int cacheSize;//cache大小
    int maxPageNum;//Cache能存储的页数

    vector<Page*> pages;//cache中存粗的page,size为cache中已有的页数
    map<int, int> pageMap;//pageID拿到pages中的index
    Page* getPageFromFile(int pageID);//从database file中获得page
    Page* getPageFromCache(int pageID);
    void addPageIntoCache(Page* page);
    void removePageByLRU();//使用LRU作为页面置换算法,置换的时候还要看该页是否被写过了,写过了就写回文件,否则直接替换即可
    
public:
    PageCache(string databaseName);
    Page* readPage(int pageID);//读Page,如果缺页,页面置换算法,返回Page*
    bool writePage(int pageID, BYTE* start, int pos, int size);//写Page,应该包括修改Page哪部分的哪些内容
    bool releasePage(int pageID);
    bool close();
    void updatePageMap();
    
    void Print(string str);
};