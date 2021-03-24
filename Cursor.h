#include "Table.h"
#include "Pager.h"

class TableCursor
{
private:
    Table* tab;
    int rowID;
    Node* node;
    Pager* pager;
    bool moveToSon(Cell key);
public:
    TableCursor(Table* t, Pager* pgr);
    
    //将游标移动到特定的key的位置
    bool moveToUnpacked(string skey);
    bool moveToNext();
    bool moveToLast();
    bool moveToSon();

    bool insert(list<string> rowctx);
    bool deleteVal();

    int getCell_i(string columnName);
    char getCell_c(string columnName);
    string getCell_s(string columnName);

    vector<string> getData();
    bool setData(list<string> newData);
};