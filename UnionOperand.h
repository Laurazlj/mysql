#pragma once
#ifndef	 _UnionOperand_
#define  _UnionOperand_
#include "common.h"
#include "Type.h"

class UnionOperand //表达式相关
{
private:
	string value;
	BasicType type;
public:
	UnionOperand(BasicType pType, string pValue);
	UnionOperand();
	BasicType getType();
	string getValue();
};
#endif