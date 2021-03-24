#pragma once
#include "Pager.h"
#include "Column.h"
#include "Node.h"

class TableCursor;
class Table
{
    char tabName[NAMESIZE];

    Column* keyCol;//主键列
    map<string, int> colMap;//从名字拿到col的index
    vector<Column> columns;

    int rootNodePageID;//Table存储的数据的第一个pageid
    int pageID;
    Pager* pager;
public:
    Table(int pID, Pager* pgr);
    Table(string tableName, Column kcol, vector<Column> cols, Pager* pgr);//对于这种情况,就会有一个写page的操作
    ~Table();
    Column getColumn(string colName);
    int getPageID();
    int getTotalPageNumFromFile();
    Node* getRootNode();
    int getRootNodePageID();
    bool updateRootNodeInfo(int rootNodePID);
    bool writeBackToPage();
    TableCursor createCursor();
	list<string> getColumnsName();
    void Print();
};
