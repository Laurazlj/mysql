#pragma once
#include "common.h"
#include <iostream>
// #include <string>
#include <fstream>
#include "Page.h"
using namespace std;
class header
{
public:
    int lastid;
    list<int> FIlereleasedPageID;
    header();
};

bool OpenDB(string path,string dbname);
BYTE* FindPageFromFile(int pageId);//从文件中读取相应ID的页的内容
void WriteBack(int pageId, BYTE* pagecontent);//将页写回文件
bool CreateDataBase(string name,string path);//创建数据库
bool DeleteDB(string path,string DBname);//删除数据库
void closeDataBase(string name,string path);//关闭数据库
int lastID();//返回文件中最后一个页的ID（页号从0开始）
list<int> FIleGetReleasedID();
void FIleWriteUnusedID(list<int>ReID);
bool FileNotExist(string dbname);

void WriteBacktest(int pageId);
void writeheader(string allpath,header thisheader);
void printPCharByTwo_(BYTE* pchr);
