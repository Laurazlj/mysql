#pragma once
#ifndef	 _VM_
#define  _VM_
#include "Expression.h"
#include "InsertRecord.h"
#include "EvalDiscription.h"
#include "Database.h"
#include <iostream>
#include "GenByteCode.h"
enum Activity 
{
	None,
	Ins,
	Del,
	Upd,
	Sel,
	Createtable,
	Droptable,
	Createdb,
	Dropdb,
	Qu
};

extern bool equals(Column col1, Column col2);
extern void run(Instruction instruction);
extern void ConditonClear();
extern void join(string tableName);
extern void execute();
extern BasicType BType(string strVal);
extern UnionOperand eval(vector<EvalDiscription> evalDiscriptions, TableCursor p);
extern bool check(TableCursor p);
extern void UseDatabase(string target);
extern void selectstar();
extern void select();
extern void _delete();
extern void update();
extern void insert();
extern void CreateTable();
extern void dropTable();
extern void CreateDatabase();
extern void DropDatabase();
extern void QuitDatabase();
extern void showtable();
extern void virtualmachine(vector<FourVarCode> fourVarCodeTab);

#endif
