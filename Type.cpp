#include "Type.h"

Type::Type(): isInt(false), isChar(false), isString(false), characterNum(0)
{
}

Type::Type(bool isint, bool isch, bool isstr): 
    isInt(isint), isChar(isch), isString(isstr), characterNum(0)
{
}

void Type::setToChar()
{
    isChar = true;
}

void Type::setToInt()
{
    isInt = true;
}

void Type::setToString(int cNum)
{
    isString = true;
    characterNum = cNum;
}

BasicType Type::getType()
{
    if(isChar) return CHARTYPE;
    if(isInt) return INTTYPE;
    if(isString) return STRINGTYPE;
    return UNKOWNTYPE;
}

int Type::getCharacterNum()
{
    return characterNum;
}

void Type::writePage(Pager* pager, int pageID, int* pos)
{
    int t = getType();
    pager->writePage(pageID, (BYTE*)&t, *pos, sizeof(int));
    *pos = *pos + sizeof(int);
    pager->writePage(pageID, (BYTE*)&characterNum, *pos, sizeof(int));
    *pos = *pos + sizeof(int);
}

void Type::readPage(Pager* pager, int pageID, int* pos)
{
    int t;
    BYTE* buffer = pager->readPage(pageID);
    memcpy(&t, buffer+(*pos), sizeof(int));
    *pos = *pos + sizeof(int);
    if(t == CHARTYPE) isChar = true;
    else if(t == INTTYPE) isInt = true;
    else if(t == STRINGTYPE) isString = true;

    memcpy(&characterNum, buffer+(*pos), sizeof(int));
    *pos = *pos + sizeof(int);
}