#pragma once
#include "Cell.h"
#include "Pager.h"
/**
 * |    Name    |   Age |
 * |    Qiu     |   21  |//这就为一个row,也就是一个row实体
 */ 
class Row
{
private:
    vector<Cell> cells;
public:
    Row(list<string> ctx);
    Row();
    Cell getCell(int index);
    vector<string> getStringVals();
    void writePage(Pager* pager, int pageID, int* pos);
    void readPage(Pager* pager, int pageID, int* pos);
    void Print();
};
