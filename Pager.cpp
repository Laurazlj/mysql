#include "Pager.h"

Pager::Pager(string databaseName)
{
    dbName = databaseName;
    pageCache = new PageCache(dbName);
    readReleasedPageIDFromFile();
    readTheNextUnusedPageIDFromFile();
}

BYTE* Pager::readPage(int pageID)
{
    return pageCache->readPage(pageID)->getPageBuffer();
}

bool Pager::writePage(int pageID, BYTE* src, int pos, int size)
{
    return pageCache->writePage(pageID, src, pos, size);
}

bool Pager::isNewDatabase(string dbname)
{
    return FileNotExist(dbname);
}

bool Pager::close()
{
 
    writeReleasedPageIDToFile(releasedPageID);
    //writeTheNextUnusedPageIDToFile();
    pageCache->close();
    cout<<"-----------db NAME!!!"<<dbName<<"-----------"<<endl;
    closeDataBase(dbName,DBFOLD);
    return true;
}

int Pager::getUnusedPageID()
{
    int res = -1;
    if(releasedPageID.size() == 0)
    {
        res = theNextUnusedPageID;
        theNextUnusedPageID++;
        return res;
    }
    res = releasedPageID.front();
    releasedPageID.pop_front();
    return res;
}

void Pager::readReleasedPageIDFromFile()
{
    releasedPageID.clear();
    releasedPageID = FIleGetReleasedID();
    //@TODO:调用卓尼的接口读取
}

bool Pager::writeReleasedPageIDToFile(list<int>ReID)
{
    //@TODO:调用卓尼的接口
    FIleWriteUnusedID(ReID);

    return true;
}

void Pager::readTheNextUnusedPageIDFromFile()
{
    //@TODO:调用卓尼的接口读取
    theNextUnusedPageID = lastID();
}

bool Pager::writeTheNextUnusedPageIDToFile()
{
    //@TODO:调用卓尼的接口
    return false;
}

bool Pager::releasePage(int pageID)
{
    releasedPageID.push_back(pageID);
    pageCache->releasePage(pageID);
    return false;
}
