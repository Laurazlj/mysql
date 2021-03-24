#pragma once
#include "Row.h"
#include "Pager.h"

#define M 3

class Node
{
private:
    vector<int> sonNodes;
    int fatherNodeID;
    int order;
    vector<Row> rows;
    Pager* pager;
    int pageid;
    //BYTE* pagebyte;// 每个Node表示一个Page
   
public:
    Node(int setpageID, Pager* pgr);//使用pager中的方法读取Node
    Node(vector<Row> thisRowList, vector<int> thisSonList, int thisOrder, Pager* pgr);
    Node(Row r, Pager* pgr, int* pageID);//针对Table中第一条数据的情况,创建page,pageID是出口函数
    vector<Row> getRows();//等同于getRowList
    Row getRow(int index);
    vector<int> getSons();//等同于getSonNodeList
    Node* getSpecialSonNode(int sonOrder);
    Node* getFatherNode();
    int getFatherNodeID();
    int getOrder();
    Row getFirstRow();
    Row getLastRow();
    
    //以下函数进行了修改，需要维护信息
    bool insertRow(Row row);
    bool setFather(int ID);  
    bool setOrder(int ord);
    vector<Row> subRowList(vector<Row>list, int a, int b);
    vector<int> subNodeList(vector<int>list, int a, int b);
    bool rootDevideNode();
    bool adjustNode(int sonOrder);

    bool deleteRow(Cell key);
    bool deleteNode(int sonOrder);

    //向page中写回修改
    void writebacktopage(int pageID);

    bool drop();

};
