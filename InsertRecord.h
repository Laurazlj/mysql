#pragma once
#include "common.h"
class InsertRecord //Ҫ����ļ�¼
{
public:
	string colname; //����
	string type; //��������
	string value; //ֵ
	InsertRecord(string colname, string type, string value);
	InsertRecord();
	string getcolname();
	string getvalue();
};