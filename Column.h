#pragma once
#ifndef	 _Column_
#define  _Column_
#include "common.h"
#include "Type.h"

class Column
{
private:
    Type colType;//该列数据类型
    char colName[NAMESIZE];
    int colNum;//表中第几列,从0开始
public:
    Column(Type type, string name, int columnNum);//种类,名字,第几列
    Column();
    void setColName(string name);
    void setColNum(int columnNum);
    void setTypeToInt();
    void setTypeToChar();
    void setTypeToString(int cNum);
    void setType(Type t);
    string getColName();
    Type getColType();
    int getColNum();
    void writePage(Pager* pager, int pageID, int* pos);
    void readPage(Pager* pager, int pageID, int* pos);
};
#endif