#include "Cell.h"

Cell::Cell(int v)
{
    memset(val, 0, NAMESIZE);
    strcpy(val, std::to_string(v).c_str());
}

Cell::Cell(char v)
{
    memset(val, 0, NAMESIZE);
    strcpy(val, std::to_string(v).c_str());
}

Cell::Cell(string v)
{
   memset(val, 0, NAMESIZE);
   strcpy(val, v.c_str());
}

Cell::Cell()
{
    memset(val, 0, NAMESIZE);
}

bool Cell::operator ==(const Cell& c)
{
    if(strcmp(val, c.val) == 0) return true;
    return false;
}

bool Cell::operator >(const Cell& c)
{
    if(strcmp(val,  c.val) > 0) return true;
    return false;
}

bool Cell::operator >=(const Cell& c)
{
    if(strcmp(val, c.val) >= 0) return true;
    return false;
}

bool Cell::operator <(const Cell& c)
{
    if(strcmp(val, c.val) < 0) return true;
    return false;
}

bool Cell::operator <=(const Cell& c)
{
    if(strcmp(val, c.val) <= 0) return true;
    return false;
}

bool Cell::operator !=(const Cell& c)
{
    if(strcmp(val, c.val) != 0) return true;
    return false;
}

string Cell::getStringValue()
{
    return val;
}

int Cell::getIntValue()
{
    return std::stoi(val);
}

char Cell::getCharValue()
{
    return val[0];
}

void Cell::writePage(Pager *pager, int pageID, int* pos)
{
    /*int strlen = val.size();
    pager->writePage(pageID, (BYTE*)&strlen, *pos, sizeof(int));
    *pos = *pos + sizeof(int);*/
    pager->writePage(pageID, (BYTE*)val, *pos, NAMESIZE*sizeof(char));
    *pos = *pos + NAMESIZE*sizeof(char);
}

void Cell::readPage(Pager *pager, int pageID, int* pos)
{
    //int strlen;
    BYTE* buffer = pager->readPage(pageID);
    //memcpy(&strlen, buffer+(*pos), sizeof(int));
    //*pos = *pos + sizeof(int);

    //char *ctx = (char*)malloc(strlen*sizeof(char));
    memcpy(val, buffer+(*pos), NAMESIZE*sizeof(char));
    //val = ctx;
    *pos = *pos + NAMESIZE*sizeof(char);
}
