#pragma once
#include "common.h"
#include "Pager.h"

class Cell{
private:
    char val[NAMESIZE];//用string的形式保存val
public:
    Cell();
    Cell(int v);
    Cell(char v);
    Cell(string v);
    void writePage(Pager *pager, int pageID, int* pos);
    void readPage(Pager *pager, int pageID, int* pos);
    bool operator ==(const Cell& c);
    bool operator >(const Cell& c);
    bool operator >=(const Cell& c);
    bool operator <(const Cell& c);
    bool operator <=(const Cell& c);
    bool operator !=(const Cell& c);
    string getStringValue();
    int getIntValue();
    char getCharValue();
};
