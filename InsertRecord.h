#pragma once
#include "common.h"
class InsertRecord //要插入的记录
{
public:
	string colname; //列名
	string type; //数据类型
	string value; //值
	InsertRecord(string colname, string type, string value);
	InsertRecord();
	string getcolname();
	string getvalue();
};