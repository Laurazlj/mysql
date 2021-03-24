#include "Table.h"
#include "Cursor.h"

void printPCharByTwo(BYTE* pchr)
{
    for(int i=0;i<PAGESIZE*sizeof(char);i++)
    {
        for(int j=7; j>=0; j--)
        {
            char tmpp = pchr[i];
            tmpp=pchr[i]&(1<<j);
            printf("%d", tmpp>>j);
        }
        printf(" ");
    }
    printf("\n");
}

Table::Table(int pID, Pager* pgr)
{
    pageID = pID;
    pager = pgr;

    int pos = 0;
    BYTE* tmpBuffer;
    tmpBuffer = pager->readPage(pageID);
    memset(tabName, 0, NAMESIZE*sizeof(char));
    memcpy(tabName, tmpBuffer+pos, NAMESIZE*sizeof(char));
    printf("tabName: %s\n", tabName);
    pos += NAMESIZE*sizeof(char);

    //读取key
    keyCol = new Column();
    keyCol->readPage(pager, pageID, &pos);

    //读取cols
    int colsNum;//列数量
    memcpy(&colsNum, tmpBuffer+pos, sizeof(int));
    printf("colsNum: %d\n", colsNum);
    pos += sizeof(int);

    for(int i = 0; i < colsNum; i++)
    {
        Column tmpCol;
        tmpCol.readPage(pager, pageID, &pos);
        columns.push_back(tmpCol);
        colMap[tmpCol.getColName()] = i;
    }
    
    memcpy(&rootNodePageID, tmpBuffer+pos, sizeof(int));
}

Table::Table(string tableName, Column kcol, vector<Column> cols, Pager* pgr)
{
    pager = pgr;
    pageID = pager->getUnusedPageID();

    strcpy(tabName, tableName.c_str());
    keyCol = new Column(kcol.getColType(), kcol.getColName(), kcol.getColNum());
    columns.assign(cols.begin(), cols.end());
    rootNodePageID = -1;
    writeBackToPage();
}

void Table::Print()
{
    printf("table Name: %s\n", tabName);
    for(vector<Column>::iterator it = columns.begin(); it != columns.end(); it++)
    {
        printf("colName: %s\n", it->getColName().c_str());
    }
}

//将Table内容写入pageBuffer
bool Table::writeBackToPage()
{
    int colsNum = columns.size();//列数量
    int pos = 0;
    pager->writePage(pageID, (BYTE*)tabName, pos, NAMESIZE*sizeof(char)); pos += NAMESIZE;
    keyCol->writePage(pager, pageID, &pos);
    pager->writePage(pageID, (BYTE*)&colsNum, pos, sizeof(int)); pos += sizeof(int);
    for(int i = 0; i < colsNum; i++)
    {
        columns[i].writePage(pager, pageID, &pos);
    }
    pager->writePage(pageID, (BYTE*)&rootNodePageID, pos, sizeof(int));
    return true;
}

Table::~Table()
{
    delete keyCol;
}

Column Table::getColumn(string colName)
{
    return columns[colMap[colName]];
}

int Table::getPageID()
{
    return pageID;
}

int Table::getRootNodePageID()
{
    return rootNodePageID;
}

Node* Table::getRootNode()
{
    if(rootNodePageID < 0) return NULL;
    else return new Node(rootNodePageID, pager);
}

bool Table::updateRootNodeInfo(int rootNodePID)
{
    rootNodePageID = rootNodePID;
    writeBackToPage();
    return true;
}

TableCursor Table::createCursor()
{
    TableCursor res(this, pager);
    return res;
}

list<string> Table::getColumnsName()
{
	list<string> sList;
	for (int i = 0; i < columns.size(); i++) 
	{
		sList.push_back(columns[i].getColName());
	}
	return sList;
}
