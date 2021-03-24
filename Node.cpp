#include "Node.h"
#include <iostream>

Node::Node(int setpageID, Pager* pgr)
{
    //使用pager中的方法读取Node
    int pos=0;
    BYTE* tmpBuffer;
    pager=pgr;
    pageid=setpageID;
    
    //读取sonnodesize
    int sonNodesize;
    tmpBuffer = pgr->readPage(setpageID);
    memcpy(&sonNodesize, tmpBuffer+pos, sizeof(int)); 
    pos+=sizeof(int);
    //读取sonnode中的元素
    for(int i=0;i<sonNodesize;i++)
    {
        int tmpsonnodes;
        memcpy(&tmpsonnodes,tmpBuffer+pos,sizeof(int));
        sonNodes.push_back(tmpsonnodes);
        pos+=sizeof(int);
    }

    //读fatherid
    memcpy(&fatherNodeID,tmpBuffer+pos,sizeof(int)) ;
    pos+=sizeof(int);
    
    //读order
    memcpy(&order,tmpBuffer+pos,sizeof(int));
    pos+=sizeof(int);

    //读rowsize 和row的元素
    int tmprowsize;
    memcpy(&tmprowsize, tmpBuffer+pos, sizeof(int));
    pos+=sizeof(int);

    //对于row的读取部分,应该在row中完成
    for(int i=0; i<tmprowsize; i++)
    {
        Row r;
        r.readPage(pgr, setpageID, &pos);
        rows.push_back(r);
    }
}
Node::Node(vector<Row> thisRowList, vector<int> thisSonList, int thisOrder, Pager* pgr)
{
    //给rows和sons 新建节点，用于分裂
       // ByteBuffer buffer = ByteBuffer.allocate(Page.PAGE_SIZE);
        pageid = pgr->getUnusedPageID();
        
        //rows = thisRowList;
        for(int i=0;i<thisRowList.size();i++)
        {
           rows.push_back(thisRowList[i]); 
        }
        //sonNodes = thisSonList;
        for(int i=0;i<thisSonList.size();i++)
        {
           sonNodes.push_back(thisSonList[i]); 
        }
        fatherNodeID = -1;
        this->pager = pgr;
        //pageOne = new Page(buffer);
        // 为每一位儿子维护父亲和排位信息。
        for (int i = 0; i < sonNodes.size(); i++)
        {
            Node* sonNode = new Node(sonNodes[i],pager);
            sonNode->setFather(pageid);
            sonNode->setOrder(i);
            
        }

        // 维护自身排位信息。
        order = thisOrder;
        

        // 序列化信息至buffer
        //pager->writePage(pageid,pagebyte, 0,PAGESIZE);
        writebacktopage(pageid);
        //Node* tmpnode = new Node(pageid, pgr);
        //cout<<" "<<endl;
} 

Node::Node(Row r, Pager* pgr, int* pageID)
{
    fatherNodeID = -1;
    order = -1;
    rows.push_back(r);
    pager = pgr;
    *pageID = pgr->getUnusedPageID();
    pageid=*pageID;

    //将Node的内容写到Page中
    int tmpSize, pos = 0;
    tmpSize =  sonNodes.size();
    pager->writePage(*pageID, (BYTE*)&tmpSize, pos, sizeof(int)); pos+=sizeof(int);
    for(int i=0; i<sonNodes.size(); i++)
    {
        pager->writePage(*pageID, (BYTE*)&sonNodes[i], pos, sizeof(int));
        pos += sizeof(int);
    }

    pager->writePage(*pageID, (BYTE*)&fatherNodeID, pos, sizeof(int)); pos+=sizeof(int);
    pager->writePage(*pageID, (BYTE*)&order, pos, sizeof(int)); pos+=sizeof(int);
    
    tmpSize = rows.size();
    pager->writePage(*pageID, (BYTE*)&tmpSize, pos, sizeof(int)); pos+=sizeof(int);
    for(int i=0; i<rows.size(); i++)
    {
        //不能对row对象做读写操作,因为row对象中有vector
        //调用row的写page函数
        rows[i].writePage(pager, *pageID, &pos);//在函数内部对pos的值进行修改
    }
}

void Node::writebacktopage(int pageID)
{
    
    //需要把所有的再写回至page中
    //将Node的内容写到Page中  需要解决pageid的问题
    pageid=pageID;

    int tmpSize, pos = 0;
    tmpSize =  sonNodes.size();
    pager->writePage(pageID, (BYTE*)&tmpSize, pos, sizeof(int)); pos+=sizeof(int);
    for(int i=0; i<sonNodes.size(); i++)
    {
        pager->writePage(pageID, (BYTE*)&sonNodes[i], pos, sizeof(int));
        pos += sizeof(int);
    }

    pager->writePage(pageID, (BYTE*)&fatherNodeID, pos, sizeof(int)); pos+=sizeof(int);
    pager->writePage(pageID, (BYTE*)&order, pos, sizeof(int)); pos+=sizeof(int);
    
    tmpSize = rows.size();
    
    pager->writePage(pageID, (BYTE*)&tmpSize, pos, sizeof(int)); pos+=sizeof(int);
    cout<<"rowsize:  "<<rows.size()<<endl;
    for(int i=0; i<rows.size(); i++)
    {
        cout<<"rows : "<<endl;
        rows[i].Print();
        int tmppos = pos;
        rows[i].writePage(pager, pageID, &pos);//在函数内部对pos的值进行修改
        Row rowtest;
        rowtest.readPage(pager, pageID, &tmppos);
        cout<<"---------------"<<endl;
        rowtest.Print();
    }
}

bool Node::insertRow(Row row)
{
    cout<<"---insertrow-----"<<endl;
    //row.Print();
    cout<<pageid<<endl;

    bool insertOrNot=false;
    int insertnum=0;
    for(int i=0;i<rows.size();i++)
    {
        if(rows[i].getCell(0)==row.getCell(0))
        {
            return false;
        }
        else if(rows[i].getCell(0)>row.getCell(0))
        {
            insertnum=i;//插入到i处
            insertOrNot=true;
            break;
        }
    }
    if(!insertOrNot)
    {
        insertnum=rows.size();//如果全都比要插入的元素小，则插入到尾部
    }
    if((sonNodes.size()==0)||(sonNodes.empty()))
    {
        //sonnodes为空的情况
        //cout<<"i:  "<<insertnum<<endl;
        rows.insert(rows.begin()+insertnum,row);
        // cout<<"newrow"<<endl;
        // rows[insertnum].Print();
        writebacktopage(pageid);
        //cout<<"writeback!!"<<endl;
        if(rows.size()<=M)
        {
            // 当未超出长度时，插入完毕
            return true;
        }
        else
        {
            // 超出长度时，进行单元分裂。
            if(fatherNodeID<0)
            {
                cout<<"rootDevideNode"<<endl;
                return rootDevideNode();
            }
            else
            {
                return new Node(fatherNodeID,pager);
            }
            
        }
        
    }
    else
    {
        
        return new Node(sonNodes[insertnum],pager);
        
    }
   // writebacktopage(pageid);
    
}
vector<Row> Node::getRows()
{
   
    return rows;
    
}
Row Node::getRow(int index)
{
    if(rows.size()>0)
    {
        return rows[index]; 
    }
    else
    {
       // cout<<"get row failed"<<endl;
       printf("get row failed");
       list<string> ctx;
       Row r(ctx);
       return r;
    }
    
}

vector<int> Node::getSons()
{
    //等同于getSonNodeList
    return sonNodes;
}

Node* Node::getSpecialSonNode(int sonOrder)
{
    return new Node(sonNodes[sonOrder],pager);

}

Node* Node::getFatherNode()
{
    return new Node(fatherNodeID,pager);
}

//？？？？？？？？？？？？
bool Node::drop()
{
    //TODO ： 递归清理节点
    //遍历B树并清理 
    
    return true;
}

bool Node::deleteRow(Cell key)
{
    bool deleteOrNot = false;
    int deletenum=0;
    for(int i=0;i<rows.size();i++)
    {
        Row thisrow=rows[i];
        if(thisrow.getCell(0)==key)
        {
            if((sonNodes.size()==0)||sonNodes.empty())
            {
                rows.erase(rows.begin()+i);
                writebacktopage(pageid);
                if(rows.size()<M/2)
                {
                    if(fatherNodeID<0)
                    {
                        if((rows.size()<1)&&(sonNodes.size()<1))
                        {
                            return true;
                        }else if(rows.size()<1)
                        {
                            rows =(new Node(sonNodes[0],pager))->rows;
                            sonNodes = (new Node(sonNodes[0],pager))->sonNodes;
                            writebacktopage(pageid);
                            
                            return true;
                        }
                        else
                        {
                            return true;
                        }
                        
                    }
                    else
                    {
                        Node nodefather=Node(fatherNodeID,pager);
                        cout<<"fatherid: "<<fatherNodeID<<endl;
                       // cout<<"nodefather.sonNodes.size: "<<nodefather.sonNodes.size()<<endl;
                        nodefather.sonNodes.erase(nodefather.sonNodes.begin() +order);
                        cout<<"nodefather.sonNodes.size: "<<nodefather.sonNodes.size()<<endl;
                        for(int i=0;i<nodefather.sonNodes.size();i++)
                        {
                            cout<<nodefather.sonNodes[i]<<endl;
                        }
                         nodefather.writebacktopage(fatherNodeID);
                        // Node nodefather2=Node(fatherNodeID,pager);
                        // cout<<"nodefather2.sonNodes.size: "<<nodefather2.sonNodes.size()<<endl;
                        // for(int i=0;i<nodefather2.sonNodes.size();i++)
                        // {
                        //     cout<<nodefather2.sonNodes[i]<<endl;
                        // }
                        //加了这部分之后虽然删除成功，但是段错误
                        return true ;
                    }
                    
                }
                else
                {
                    return true;
                }
                
            }
            else
            {
                Row temprow=getFirstRow();
                rows.insert(rows.begin()+i,temprow);
                writebacktopage(pageid);

                return new Node(sonNodes[i+1],pager);
            }
            
        }
        else if(thisrow.getCell(0)>key)
        {
            deletenum=i;
            deleteOrNot=true;
            break;
        }
    }
    if(!deleteOrNot)
    {
        deletenum=rows.size();
    }
    if((sonNodes.size()==0))
    {
        return false;
    }
    else
    {
        return new Node(sonNodes[deletenum],pager);
    }
    
    //writebacktopage(pageid);

}

int Node::getFatherNodeID()
{
    return fatherNodeID;
}

int Node::getOrder()
{
    return order;
}

bool Node::rootDevideNode()
{
    cout<<"pageid: "<<pageid<<endl;
    
    vector<Row> leftRow;
    vector<Row> rightRow;
    vector<int> leftNode;
    vector<int> rightNode;
    leftRow = subRowList(rows,0,M/2-1);
    if(sonNodes.size() == 0)
    {
           vector<int> leftNode;
    }
    else
    {
            leftNode = subNodeList(sonNodes,0,M/2);
    }
    rightRow = subRowList(rows,M/2 + 1, M);
    if(sonNodes.size() == 0)
    {
            vector<int> rightNode;
    }
    else
    {
            rightNode =subNodeList(sonNodes,M/2 + 1, M);
    }

    rows = subRowList(rows,M/2, M/2);
    
    vector<int> newSonList;
    Node* n1 = new Node(leftRow, leftNode, 0, pager);
    n1->setFather(pageid);
    Node* n2 = new Node(rightRow, rightNode,1, pager);
    n2->setFather(pageid);
    int leftsonid=n1->pageid;
    int rightsonid=n2->pageid;
    newSonList.push_back(leftsonid);
    cout<<"sonid : "<<leftsonid<<endl;
    newSonList.push_back(rightsonid);
     cout<<"sonid : "<<rightsonid<<endl;
    //sonNodes = newSonList;
    for(int i=0;i<newSonList.size();i++)
    {
        sonNodes.push_back(newSonList[i]);
    }
    cout<<"pageid: "<<pageid<<endl;
    writebacktopage(pageid);

    return true;

}
Row  Node::getFirstRow()
{
    if( (rows.size() != 0)) {
    	
    
        if ( (sonNodes.size() == 0))
        {
            return rows[0];
        }
        else
        {
            return (new Node(sonNodes[0],pager))->getFirstRow();
        }
    }
    else
    {
        printf("get first row failed because rows is null");
        list<string> ctx;
        Row r(ctx);
        return r;
    }
}
vector<Row> Node::subRowList(vector<Row>list, int a, int b)
{
        vector<Row> ret;
        for(int i = a; i <= b; i++)
        {
            ret.push_back(list[i]);
        }
        return ret;
}

vector<int> Node::subNodeList(vector<int>list, int a, int b)
{
       vector<int> ret ;
        for(int i = a; i <= b; i++)
        {
            ret.push_back(list[i]);
        }
        return ret;
}
bool Node::setFather(int ID) 
{
        fatherNodeID = ID;
        writebacktopage(pageid);
        return true;
}

     //除了Transaction部分
bool Node::setOrder(int ord)
{
        order = ord;
        writebacktopage(pageid);

        return true;
}
    
bool Node::deleteNode(int sonOrder)
{
   
    if (sonOrder < sonNodes.size() - 1)
    {
            Row thisRow = rows[sonOrder];
            Node* rightNode = new Node(sonNodes[sonOrder + 1],pager);
            rightNode->insertRow(thisRow);
            rows.erase(rows.begin()+sonOrder);
            sonNodes.erase(sonNodes.begin()+sonOrder);
            // 维护page信息
           // intoBytes(thisTran);
            writebacktopage(pageid);


            for (int i = sonOrder; i < sonNodes.size(); i++)
            {
                (new Node(sonNodes[i],pager))->setOrder(i);
            }
            if (rows.size() < M/2)
            {
                if (fatherNodeID < 0)
                {
                    if (rows.size() < 1)
                    {
                        rows = (new Node(sonNodes[0],pager))->rows;
                        sonNodes = (new Node(sonNodes[0],pager))->sonNodes;
                        // 维护page信息
                        //intoBytes(thisTran);
                        writebacktopage(pageid);
                        return true;
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    return (new Node(fatherNodeID,pager))->adjustNode(order);
                }
            }
            else
            {
                return true;
            }
    }
    else
    {
            Row thisRow = rows[sonOrder - 1];
            Node* leftNode = new Node(sonNodes[order-1],pager);
            leftNode->insertRow(thisRow);

            rows.erase(rows.begin()+sonOrder - 1);
            sonNodes.erase(sonNodes.begin()+sonOrder);
            // 维护page信息
            //intoBytes(thisTran);
            writebacktopage(pageid);

            for (int i = sonOrder; i < sonNodes.size(); i++)
            {
                (new Node(sonNodes[i],pager))->setOrder(i);
            }
            if (rows.size() < M/2)
            {
                if (fatherNodeID < 0)
                {
                    if (rows.size() < 1)
                    {
                        rows = (new Node(sonNodes[0],pager))->rows;
                        sonNodes = (new Node(sonNodes[0],pager))->sonNodes;
                        // 维护page信息
                        //intoBytes(thisTran);
                        writebacktopage(pageid);

                        return true;
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    return (new Node(fatherNodeID,pager))->adjustNode(order );
                }
            }
            else
            {
                return true;
            }
    }
}

bool Node::adjustNode(int sonOrder)
{
        Node *thisSonNode = new Node(sonNodes[sonOrder],pager);

        // 排除最大值边界越界情况，向左下合并
        if (sonOrder <  sonNodes.size() - 1)
        {
            Node *rightSonNode = new Node( sonNodes[sonOrder + 1],pager);
            if (rightSonNode->rows.size() > M/2)
            {
                thisSonNode->insertRow(rows[sonOrder] );
                rightSonNode->deleteRow(rows[order].getCell(0) );
                rows.insert(rows.begin()+sonOrder, rightSonNode->getFirstRow());
                //维护本page信息
                //intoBytes(thisTran);
                writebacktopage(pageid);
                return true;
            }
        }

        // 排除零值边界越界情况，向右下合并
        if (sonOrder > 0)
        {
            Node *leftSonNode = new Node( sonNodes[sonOrder - 1],pager);
            if (leftSonNode->rows.size() > M/2)
            {
                thisSonNode->insertRow(rows[sonOrder - 1] );
                leftSonNode->deleteRow(rows[order].getCell(0) );
                rows.insert(rows.begin()+sonOrder - 1, leftSonNode->getLastRow());
                //维护本page信息
               // intoBytes(thisTran);
                writebacktopage(pageid);

                return true;
            }
        }

        // 没有相邻的可支援兄弟节点，只好删除此节点。
        return deleteNode(sonOrder);
}
Row Node::getLastRow() 
{
        if ( (sonNodes.size() == 0))
        {
            return rows[rows.size() - 1];
        }
        else
        {
            return (new Node(sonNodes[sonNodes.size() - 1],pager))->getLastRow();
        }
}


//缺构造函数和drop