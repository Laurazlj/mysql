#pragma once
#ifndef	 _Instruction_
#define  _Instruction_
#include "common.h"

enum OpCode //操作码
{
	Exit,
	UseDB,
	DropDB,
	showDB,
	showTB,
	createDB,

	Createtb,
	Droptb,

	Insert,
	Delete,
	Select,
	Update,

	Set,

	Add,
	Sub,
	Div,
	Mul,
	And,
	Not,
	Or,
	GT,
	GE,
	LT,
	LE,
	EQ,
	NE,

	Addcol,
	BeginPK,
	AddPK,
	EndPK,

	BeginItem,
	AddItemCol,
	EndItem,

	BeginColSelect,
	AddColSelect,
	SelectStar,
	EndColselect,

	BeginFilter,
	Operand,
	Operator,
	EndFilter,

	BeginJoin,
	AddTable,
	EndJoin,

	Execute,
	BeginExpression,
	EndExpression,
	
	Quit
};

class Instruction //指令
{
public:
	OpCode opCode;
	string p1;
	string p2;
	string p3;
	Instruction(OpCode opCode, string p1, string p2, string p3);//构造函数
	Instruction();//默认构造函数
};

#endif