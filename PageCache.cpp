#include "PageCache.h"
#include <iostream>

PageCache::PageCache(string databaseName):
    dbName(databaseName), pageSize(PAGESIZE), cacheSize(CACHESIZE)
{
    maxPageNum = cacheSize / pageSize;
}

void PageCache::Print(string str)
{
    for(vector<Page*>::iterator it = pages.begin(); it != pages.end(); it++)
    {
        cout<<str<<" pID: "<<(*it)->getPageID()<<endl;
    }
}

Page* PageCache::getPageFromCache(int pageID)
{
    Page* rs = NULL;
    int index = pageMap[pageID];
    rs = pages[index];
    pages.erase(pages.begin()+index);
    pages.push_back(rs);
    updatePageMap();
    return rs;
}

void PageCache::updatePageMap()
{
    int cnt = 0;
    for(vector<Page*>::iterator it = pages.begin(); it != pages.end(); it++)
    {
        pageMap[(*it)->getPageID()] = cnt;
        cnt++;
    }
}

void PageCache::removePageByLRU()
{
    Page* tmpp = pages[0];
    pages.erase(pages.begin());
    pageMap.erase(pageMap.find(tmpp->getPageID()));
    updatePageMap();
    //检查该页状态,有无被写过,被写过就写回
    if(tmpp->getIsWritten())
    {
        tmpp->writeIntoFile();
    }
    //最后delete
    delete tmpp;
}

Page* PageCache::getPageFromFile(int pageID)//使用卓尼提供的接口
{
    BYTE* buffer = FindPageFromFile(pageID);
    Page* p = new Page(pageID, buffer);
    return p;
}

bool PageCache::writePage(int pageID, BYTE* src, int start, int size)
{
    Page* p = readPage(pageID);
    p->modifyPageCtx(src, start, size);
    p->setWritten();
    return true;
}

void PageCache::addPageIntoCache(Page* page)
{
    pages.push_back(page);
    pageMap[page->getPageID()] = pages.size()-1;
}

Page* PageCache::readPage(int pageID)
{
    Page* rs = NULL;
    //首先看cache中有无
    if(pageMap.find(pageID) != pageMap.end())//命中
    {
        //Print("pageID: "+std::to_string(pageID));
        return getPageFromCache(pageID);
    }
    else//没命中
    {
        if(pages.size() == maxPageNum)//使用LRU进行替换
        {
            removePageByLRU();
            rs = getPageFromFile(pageID);
            addPageIntoCache(rs);
            return rs;
        }
        else
        {
            rs = getPageFromFile(pageID);
            addPageIntoCache(rs);
            return rs;
        }
    }
}

bool PageCache::close()
{
    //关闭的时候将所有的Page写回
    while(pages.size() > 0) removePageByLRU();
    return true;
}

bool PageCache::releasePage(int pageID)
{
    if(pageMap.find(pageID) == pageMap.end()) return false;//如果pageCache中没有该页,直接返回
    int index = pageMap[pageID];
    pages.erase(pages.begin() + index);
    pageMap.erase(pageMap.find(pageID));
    updatePageMap();
    return true;
}
