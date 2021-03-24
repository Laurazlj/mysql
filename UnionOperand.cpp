#include "UnionOperand.h"
#include <sstream>

UnionOperand::UnionOperand(BasicType pType, string pValue)
{
	this->value = pValue;
	this->type = pType;
	if (type == INTTYPE)
	{
		double n = atof(value.c_str());
		stringstream ss;
		int m = (int)n;
		ss << m;
		this->value = ss.str();
	}
}

UnionOperand::UnionOperand()
{

}

BasicType UnionOperand::getType()
{
	return this->type;
}

string UnionOperand::getValue()
{
	return this->value;
}