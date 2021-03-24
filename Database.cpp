#include "Database.h"

DataBase::DataBase(string databaseName):dbName(databaseName)
{
    pager = new Pager(dbName);
    if(pager->isNewDatabase(dbName))//是否为新的数据库
    {
        //cout<<"begin create"<<endl;
        //cout<<"databasename :   "<<databaseName<<endl;
        CreateDataBase(databaseName, DBFOLD);//创建数据库
        initDatabaseInfo();//创建数据库文件,并创建系统表
    }
    else//不为新的数据库,就从page[0]中读取系统表的信息
    {
        OpenDB(DBFOLD, dbName);
        master = new Table(0, pager);//@TODO:给page[0]加读锁
        //master->Print();
    }

}

bool DataBase::initDatabaseInfo()
{
    //创建系统表
    Type kct, ct;
    kct.setToString(NAMESIZE); ct.setToInt();
    list<pair<Type, string>> colDef;//表中含有的列类型和列
    colDef.push_back(make_pair(kct, "tabName"));
    colDef.push_back(make_pair(ct, "pageID"));

    //写到databsefile里
    master = createTable("master", "tabName", colDef);

    list<string> colValues;
    colValues.push_back("master");
    colValues.push_back("0");

    master->createCursor().insert(colValues);
    return true;
}

Table* DataBase::createTable(string tabName, string keyColName, 
                                list<pair<Type, string>> colDef)
{
    //正常创建表(对master进行插入)
    vector<Column> colList;

    Column keyCol;
    for(list<pair<Type, string>>::iterator it = colDef.begin(); it != colDef.end();
            it++)
    {
        if(it->second == keyColName)
        {
            keyCol.setType(it->first);
            keyCol.setColName(it->second);
            keyCol.setColNum(0);
            break;
        }
    }
    colList.push_back(keyCol);
    for(list<pair<Type, string>>::iterator it = colDef.begin(); it != colDef.end();
            it++)
    {
        if(it->second != keyColName)
        {
            Column col(it->first, it->second, colList.size());
            colList.push_back(col);
        }
    }

    Table* ret = new Table(tabName, keyCol, colList, pager);

    if(tabName != "master") 
        insertNewTabIntoMaster(tabName, std::to_string(ret->getPageID()));

    return ret;
}

bool DataBase::insertNewTabIntoMaster(string tabName, string pageID)
{
    cout<<"------------"<<tabName<<" "<<pageID<<"--------------"<<endl;
    list<string> colValues;//列号
    colValues.push_back(tabName);
    colValues.push_back(pageID);

    master->createCursor().insert(colValues);
    return true;
}

//删除表中,对master相应内容删除,
bool DataBase::dropTable(string tabName)
{
    //根据master表中该表名对应的pageID
    TableCursor cursor = master->createCursor();
    cursor.moveToUnpacked(tabName);
    int pageID = cursor.getCell_i("pageID");
    //读取该table对应的rootNode,将它删除
    Table t(pageID, pager);
    Node* root = t.getRootNode();
    if(root != NULL)
    {
        root->drop();//将表所存的所有的数据的pageID都释放掉,释放同时要检查pagecache中有无该pageID,有的话同时释放掉
    }
    //将page从pagecache中删除
    pager->releasePage(pageID);
    //将master表所指的这条数据删除,也就是将这个表的信息从master中删除
    cursor.deleteVal();
    return true;
}

bool DataBase::close()//将pagecache中所有的内容全部写回
{
    cout<<"close"<<endl;
    //master->writeBackToPage();
    return pager->close();
}

Table DataBase::getTable(string tableName)
{
    TableCursor cursor = master->createCursor();
    cursor.moveToUnpacked(tableName);
    int pageID = cursor.getCell_i("pageID");
    Table res(pageID, pager);
    return res;
}

bool DataBase::dropDatabase(string databaseName)
{
	if (DeleteDB(DBFOLD, databaseName))
	{
		return true;
	}
	else
	{
		return false;
	}
}