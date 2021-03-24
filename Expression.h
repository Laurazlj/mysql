#pragma once
#ifndef	 _Expression_
#define  _Expression_
#include "common.h"
#include "UnionOperand.h"
#include <list>
#include "Instruction.h"

class Expression
{
private:
	list<UnionOperand> operands;
	pair<UnionOperand, UnionOperand> typeCast(pair<UnionOperand, UnionOperand> obj);
	int _EQ(UnionOperand a, UnionOperand b);
	int _NE(UnionOperand a, UnionOperand b);
	int _GE(UnionOperand a, UnionOperand b);
	int _GT(UnionOperand a, UnionOperand b);
	int _LE(UnionOperand a, UnionOperand b);
	int _LT(UnionOperand a, UnionOperand b);
	int _Add(UnionOperand a, UnionOperand b);
	int _Sub(UnionOperand a, UnionOperand b);
	int _Mul(UnionOperand a, UnionOperand b);
	int _Div(UnionOperand a, UnionOperand b);
	int _And(UnionOperand a, UnionOperand b);
	int _Or(UnionOperand a, UnionOperand b);
	int _Not(UnionOperand a);
public:
	Expression();
	void clearList();
	void addOperand(UnionOperand obj);
	void applyOperator(OpCode op);
	UnionOperand getAns();
};

#endif
