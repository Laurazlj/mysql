#include "Column.h"
Column::Column(Type type, string name, int columnNum):
    colNum(columnNum)
{
    setType(type);
    memset(colName, 0, NAMESIZE*sizeof(char));
    strcpy(colName, name.c_str());
}

Column::Column():colNum(-1)
{
    memset(colName, 0, NAMESIZE*sizeof(char));
}

string Column::getColName()
{
    string rs = colName;
    return rs;
}

Type Column::getColType()
{
    return colType;
}

int Column::getColNum()
{
    return colNum;
}

void Column::setColName(string name)
{
    strcpy(colName, name.c_str());
}

void Column::setColNum(int columnNum)
{
    colNum = columnNum;
}

void Column::setTypeToInt()
{
    colType.setToInt();
}

void Column::setTypeToChar()
{
    colType.setToChar();
}

void Column::setTypeToString(int cNum)
{
    colType.setToString(cNum);
}

void Column::setType(Type t)
{
    if(t.getType() == INTTYPE) setTypeToInt();
    else if(t.getType() == CHARTYPE) setTypeToChar();
    else if(t.getType() == STRINGTYPE) setTypeToString(t.getCharacterNum());
}

void Column::writePage(Pager* pager, int pageID, int* pos)
{
    colType.writePage(pager, pageID, pos);
    pager->writePage(pageID, (BYTE*)colName, *pos, NAMESIZE*sizeof(char)); *pos = *pos +  NAMESIZE*sizeof(char);
    pager->writePage(pageID, (BYTE*)&colNum, *pos, sizeof(int)); *pos = *pos + sizeof(int);
}

void Column::readPage(Pager* pager, int pageID, int* pos)
{
    colType.readPage(pager, pageID, pos);

    BYTE* buffer = pager->readPage(pageID);
    memcpy(colName, buffer+(*pos), NAMESIZE*sizeof(char));
    *pos = *pos + NAMESIZE*sizeof(char);

    memcpy(&colNum, buffer+(*pos), sizeof(int));
    *pos = *pos + sizeof(int);
}