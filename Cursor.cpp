#include "Cursor.h"
#include <iostream>

TableCursor::TableCursor(Table* t, Pager* pgr): tab(t), rowID(0), pager(pgr)
{
    node = tab->getRootNode();
}

bool TableCursor::moveToSon()
{
    while(node->getSons().size() != 0)
    {
        node = node->getSpecialSonNode(0);
    }
    rowID = 0;
    return true;
}

bool TableCursor::moveToSon(Cell key)
{
    vector<Row> rows = node->getRows();
    if(rows.size() == 0) return false;
    //cout<<"key value:"<<key.getStringValue()<<endl;
    
    int index = 0;
    for(vector<Row>::iterator it = rows.begin(); it != rows.end(); it++)
    {
        //it->Print();
        if(key == it->getCell(0))//Row的第一个Cell就是key
        {
            rowID = index;//将游标移到这个位置
            return true;
        }
        index++;
    }
    
    //如果没找到,则在它的子节点中查找
    index = 0;
    if(node->getSons().size() == 0) return false;
    //如果大于key,则在右边进行搜索
    //如果key大于所有子节点的key值,那么直接在最右边进行搜索
    for(vector<Row>::iterator it = rows.begin(); it != rows.end(); it++)
    {
        if(key < it->getCell(0))
        {
            node = node->getSpecialSonNode(index);
            return moveToSon(key);
        }
        index++;
    }
    if(key > rows[rows.size()-1].getCell(0))
    {
        node = node->getSpecialSonNode(rows.size()-1);
        return moveToSon(key);
    }
    return false;
    
}

//将游标移动到特定位置
bool TableCursor::moveToUnpacked(string skey)
{
    //cout<<"moveToUnpacked "<<skey<<endl;
    //cout<<tab->getRootNodePageID()<<endl;
    node = tab->getRootNode();
    if(node == NULL) return false;
    rowID = 0;
    Cell c(skey);
    moveToSon(c);
    return true;
}

//游标移动到末尾
bool TableCursor::moveToLast()
{
    while(node->getSons().size() != 0)
    {
        node = node->getSpecialSonNode(node->getSons().size()-1);
    }
    rowID = node->getSons().size()-1;
    return true;
}

bool TableCursor::moveToNext()
{
    if(node == NULL) return false;
    //如果当前节点还有下一个数据項
    if(rowID < node->getRows().size() - 1)
    {
        rowID++;
        return true;
    }

    while(node->getFatherNodeID() > 0)
    {
        //如果当前节点不是其父节点的最后一个儿子节点
        //找它的父节点的下一个节点的最后一个叶子节点
        if(node->getOrder() < node->getFatherNode()->getSons().size() - 1)
        {
            node = node->getFatherNode()->getSpecialSonNode(node->getOrder()+1);
            while(node->getSons().size() != 0)
            {
                node = node->getSpecialSonNode(0);
            }
            rowID = 0;
            return true;
        }
        else
        {
            node = node->getFatherNode();
            rowID = 0;
            return true;
        }
    }
    moveToLast();
    return false;
}

bool TableCursor::insert(list<string> rowctx)
{
    Row row(rowctx);
    if(tab->getRootNodePageID() == -1)//如果该表数据項为空,为其创建一个记录数局的页,页是按照B树的形式进行存储(也就是Node)
    {
        int pageID;
        Node rootNode(row, pager, &pageID);
        //把rootNode内容写回
        tab->updateRootNodeInfo(pageID);
    }
    else
    {
        tab->getRootNode()->insertRow(row);
    }
    
    //将游标移动到插入的这条数据
    //cout<<row.getCell(0).getStringValue()<<endl;
    return moveToUnpacked(row.getCell(0).getStringValue());
}

//删除本条数据,返回值表示能否移动到下一条数据
bool TableCursor::deleteVal()
{
    if(node == NULL) return false;
    Cell keyCell = node->getRow(rowID).getCell(0);//得到这条数据的key
    Node* curNode = node;
    bool flag = moveToNext();//将游标移动到下一条数据
    /*
    Cell nextCell = null;
    if (thisNode != null) {
        nextCell = thisNode.getRow(thisRowID).getCell(0);
    } else {
        nextCell = null;
    }

    aimTable.getRootNode(thisTran).deleteRow(keyCell, thisTran);
    Node rootNode = aimTable.getRootNode(thisTran);
    
    if(rootNode.getFirstRow(thisTran) == null) {
        thisNode = null;
    }

    if (thisNode != null) {
        moveToUnpacked(thisTran, nextCell.getValue_s());
    }
     */ 
    curNode->deleteRow(keyCell);
    return flag;
}

int TableCursor::getCell_i(string columnName)
{
    return node->getRow(rowID).getCell(tab->getColumn(columnName).getColNum()).getIntValue();
}

char TableCursor::getCell_c(string columnName)
{
    return node->getRow(rowID).getCell(tab->getColumn(columnName).getColNum()).getCharValue();
}

string TableCursor::getCell_s(string columnName)
{
    return node->getRow(rowID).getCell(tab->getColumn(columnName).getColNum()).getStringValue();
}

vector<string> TableCursor::getData()
{
    vector<string> res;
    res.clear();

    if(node == NULL) return res;
    return node->getRow(rowID).getStringVals();
}

bool TableCursor::setData(list<string> newData)
{
    if(!deleteVal()) return false;
    if(!insert(newData)) return false;
    return true;
}