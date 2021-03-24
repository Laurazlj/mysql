#include "common.h"
#include "Pager.h"
#include "Table.h"
#include "Cursor.h"
#include "dbfile.h"

class DataBase
{
private:
    string dbName;//数据库名
    Pager* pager;
    Table* master;//数据库的表对应的pageID,相当于page[0]的信息

    bool initDatabaseInfo();
    bool insertNewTabIntoMaster(string tabName, string pageID);
public:
    DataBase(string databaseName);
    ~DataBase();
    bool close();
    Table* createTable(string tabName, string keyCol, list<pair<Type, string>> colDef);
    bool dropTable(string tabName);
    Table getTable(string tableName);
	bool dropDatabase(string databaseName);
};