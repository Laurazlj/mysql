#pragma once
#ifndef	 _CHANGE_
#define  _CHANGE_
#include "common.h"
#include "Instruction.h"

map<string, OpCode> trans;
map<string, OpCode> operators;
OpCode string_to_OpCode(string str);
OpCode operator_to_OpCode(string str);

void generatemap() //stringת��Ϊopcode������Map
{
	trans.insert(make_pair("Exit", Exit));
	trans.insert(make_pair("UseDB", UseDB));
	trans.insert(make_pair("DropDB", DropDB));
	trans.insert(make_pair("showDB", showDB));
	trans.insert(make_pair("showTB", showTB));
	trans.insert(make_pair("createDB", createDB));
	trans.insert(make_pair("Createtb", Createtb));
	trans.insert(make_pair("Droptb", Droptb));
	trans.insert(make_pair("Insert", Insert));
	trans.insert(make_pair("Delete", Delete));
	trans.insert(make_pair("Select", Select));
	trans.insert(make_pair("Update", Update));
	trans.insert(make_pair("Set", Set));
	trans.insert(make_pair("Addcol", Addcol));
	trans.insert(make_pair("BeginPK", BeginPK));
	trans.insert(make_pair("AddPK", AddPK));
	trans.insert(make_pair("EndPK", EndPK));
	trans.insert(make_pair("BeginItem", BeginItem));
	trans.insert(make_pair("AddItemCol", AddItemCol));
	trans.insert(make_pair("EndItem", EndItem));
	trans.insert(make_pair("BeginColSelect", BeginColSelect));
	trans.insert(make_pair("AddColSelect", AddColSelect));
	trans.insert(make_pair("SelectStar", SelectStar));
	trans.insert(make_pair("EndColselect", EndColselect));
	trans.insert(make_pair("BeginFilter", BeginFilter));
	trans.insert(make_pair("Operand", Operand));
	trans.insert(make_pair("Operator", Operator));
	trans.insert(make_pair("EndFilter", EndFilter));
	trans.insert(make_pair("BeginJoin", BeginJoin));
	trans.insert(make_pair("AddTable", AddTable));
	trans.insert(make_pair("EndJoin", EndJoin));
	trans.insert(make_pair("Execute", Execute));
	trans.insert(make_pair("BeginExpression", BeginExpression));
	trans.insert(make_pair("EndExpression", EndExpression));
	trans.insert(make_pair("Quit",Quit));
	operators.insert(make_pair("+", Add));
	operators.insert(make_pair("-", Sub));
	operators.insert(make_pair("/", Div));
	operators.insert(make_pair("*", Mul));
	operators.insert(make_pair(">", GT));
	operators.insert(make_pair(">=", GE));
	operators.insert(make_pair("<", LT));
	operators.insert(make_pair("<=", LE));
	operators.insert(make_pair("=", EQ));
	operators.insert(make_pair("!=", NE));
	operators.insert(make_pair("AND",And));
	operators.insert(make_pair("OR",Or));
	operators.insert(make_pair("NOT",Not));
}

OpCode string_to_OpCode(string str)
{
	return trans[str];
}

OpCode operator_to_OpCode(string str)
{
	return operators[str];
}

#endif