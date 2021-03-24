#include "Row.h"
#include <iostream>

Row::Row(list<string> ctx)
{
    for(list<string>::iterator it = ctx.begin(); it != ctx.end(); it++)
    {
        Cell tmp(*it);
        cells.push_back(tmp);
    }
}

Row::Row()
{
    cells.clear();
}

Cell Row::getCell(int index)
{
    return cells[index];
}

vector<string> Row::getStringVals()
{
    vector<string> res;
    res.clear();
    for(vector<Cell>::iterator it = cells.begin(); it != cells.end(); it++)
    {
        res.push_back(it->getStringValue());
    }
    return res;
}

void Row::Print()
{
    for(vector<Cell>::iterator it = cells.begin(); it != cells.end(); it++)
    {
        cout<<it->getStringValue()<<endl;
    }
}

void Row::writePage(Pager *pager, int pageID, int* pos)
{
    //首先写回size
    int size = cells.size();
    pager->writePage(pageID, (BYTE*)&size, *pos, sizeof(int));
    *pos = *pos + sizeof(int);
    for(int i=0; i < size; i++)
    {
        cells[i].writePage(pager, pageID, pos);
    }
}

void Row::readPage(Pager* pager, int pageID, int* pos)
{
    int size;
    BYTE* buffer = pager->readPage(pageID);
    memcpy(&size, buffer+(*pos), sizeof(int));
    *pos = *pos + sizeof(int);
    for(int i=0; i < size; i++)
    {
        Cell c;
        c.readPage(pager, pageID, pos);
        cells.push_back(c);
    }
}