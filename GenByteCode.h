#pragma once
#include "common.h"
#include <vector>
#include <map>
#include <iostream>

typedef struct
{
    string op;
    string num_a;//var1
    string num_b;//var2
    string num_c;//var3
    int line;//行号

}FourVarCode;

extern vector<FourVarCode> fourVarCodeTab;

typedef struct
{
    string fname;
    string ftype;
}fieldtype;

/**
 * 四元式表
 
typedef struct
{
    FourVarCode element[512];
    int length;//记录四元式表中的项数
}FourVarCodeTab;

FourVarCodeTab fourVarCodeTab;
*/


void insertIntoFourVarCodeTab(string op, string a, string b, string c);
vector<FourVarCode> getMidCode();
void printmidcode();
string generateVar();
void useDatabase(string dbName);
void showDatabases();
void showTables();
void createDatabase(string dbName);
void createtable(string tablename);
void AddCol(string fieldname,string typestring);
void dropDB(string dbname);
void droptb(string tablename);
//insert?   fieldtype不好确定
void inserttablename(string tablename);
void insertintotable(string tablename,vector<fieldtype> fieldv,vector<string>datav);
//select
void selectfield(string tablename,vector<string> selectfiedv);
//join
string IRjointable(vector<string>jointable);
//condition?????
void insertoprand(string constant);
void insertoperator(string IRoperator);
void insertoprand_col(string col_name);
void beginfilter();
void endfilter();
//delete
void deleteRol(string tablename);
//update
void updatetable(string tablename);
//set
void setcolname(map<string,string> updatef_dv);
//exit
void exit();
void quit();
void pk(string colname);
void selectstar(string tablename);