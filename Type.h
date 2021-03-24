#ifndef _TYPE_
#define _TYPE_
#include "Pager.h"

enum BasicType
{
    INTTYPE,
    CHARTYPE,
    STRINGTYPE,
    UNKOWNTYPE,
};

class Type
{
private:
    bool isInt;
    bool isChar;
    bool isString;//相当于char(n)
    int characterNum;//如果是string,那么字符数量
public:
    Type();
    Type(bool isint, bool isch, bool isstr);
    void setToInt();//type为int
    void setToChar();//type为char
    void setToString(int cNum);//type为string
    BasicType getType();
    int getCharacterNum();
    void writePage(Pager* pager, int pageID, int* pos);
    void readPage(Pager* pager, int pageID, int* pos);
};
#endif