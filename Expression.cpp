#include "Expression.h"

Expression::Expression()
{

}

void Expression::clearList()
{
	operands.clear();
}

void Expression::addOperand(UnionOperand obj)
{
	operands.push_front(obj);
}

void Expression::applyOperator(OpCode op)
{
	if (op == Not)
	{
		UnionOperand a = operands.front();
		operands.pop_front();
		string str = a.getValue();
		int temp = atoi(str.c_str());
		temp = not(temp);
		str = to_string(temp);
		a = UnionOperand(INTTYPE, str);
	}
	else
	{
		UnionOperand a = operands.front();
		operands.pop_front();
		UnionOperand b = operands.front();
		operands.pop_front();
		UnionOperand temp;
		switch (op)
		{
		case EQ:
			temp = UnionOperand(INTTYPE, to_string(_EQ(a, b)));
			operands.push_back(temp);
			break;
		case NE:
			temp = UnionOperand(INTTYPE, to_string(_NE(a, b)));
			operands.push_back(temp);
			break;
		case GE:
			temp = UnionOperand(INTTYPE, to_string(_GE(a, b)));
			operands.push_back(temp);
			break;
		case GT:
			temp = UnionOperand(INTTYPE, to_string(_GT(a, b)));
			operands.push_back(temp);
			break;
		case LE:
			temp = UnionOperand(INTTYPE, to_string(_LE(a, b)));
			operands.push_back(temp);
			break;
		case LT:
			temp = UnionOperand(INTTYPE, to_string(_LT(a, b)));
			operands.push_back(temp);
			break;
		case Add:
			temp = UnionOperand(INTTYPE, to_string(_Add(a, b)));
			operands.push_back(temp);
			break;
		case Sub:
			temp = UnionOperand(INTTYPE, to_string(_Sub(a, b)));
			operands.push_back(temp);
			break;
		case Mul:
			temp = UnionOperand(INTTYPE, to_string(_Mul(a, b)));
			operands.push_back(temp);
			break;
		case Div:
			temp = UnionOperand(INTTYPE, to_string(_Div(a, b)));
			operands.push_back(temp);
			break;
		case And:
			temp = UnionOperand(INTTYPE, to_string(_And(a, b)));
			operands.push_back(temp);
			break;
		case Or:
			temp = UnionOperand(INTTYPE, to_string(_Or(a, b)));
			operands.push_back(temp);
			break;
		case Not:
			temp = UnionOperand(INTTYPE, to_string(_Not(a)));
			operands.push_back(temp);
			break;
		default:
			break;
		}
	}
}

UnionOperand Expression::getAns()
{
	return this->operands.front();
}

pair<UnionOperand, UnionOperand> Expression::typeCast(pair<UnionOperand, UnionOperand> obj)
{
	UnionOperand a = obj.first;
	UnionOperand b = obj.second;
	if (a.getType() != b.getType())
	{
		if (a.getType() == INTTYPE)
		{
			if (b.getType() == STRINGTYPE)
			{
				a = UnionOperand(STRINGTYPE, a.getValue());
			}
		}
		else if (a.getType() == STRINGTYPE)
		{
			b = UnionOperand(STRINGTYPE, b.getValue());
		}
		else
		{
			if (b.getType() == STRINGTYPE)
			{
				a = UnionOperand(STRINGTYPE, a.getValue());
			}
		}
	}
	pair<UnionOperand, UnionOperand> p1(a, b);
	return p1;
}

int Expression::_EQ(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	if (obj.first.getValue() == obj.second.getValue())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Expression::_NE(UnionOperand a, UnionOperand b)
{
	return 1 - _EQ(a, b);
}

int Expression::_GE(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	int m = 0;
	int n = 0;
	string str1 = obj.first.getValue();
	string str2 = obj.second.getValue();
	m = atoi(str1.c_str());
	n = atoi(str2.c_str());
	if (n >= m)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Expression::_GT(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	int m = 0;
	int n = 0;
	string str1 = obj.first.getValue();
	string str2 = obj.second.getValue();
	m = atoi(str1.c_str());
	n = atoi(str2.c_str());
	if (n > m)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Expression::_LE(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	int m = 0;
	int n = 0;
	string str1 = obj.first.getValue();
	string str2 = obj.second.getValue();
	m = atoi(str1.c_str());
	n = atoi(str2.c_str());
	if (n <= m)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Expression::_LT(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	int m = 0;
	int n = 0;
	string str1 = obj.first.getValue();
	string str2 = obj.second.getValue();
	m = atoi(str1.c_str());
	n = atoi(str2.c_str());
	if (n < m)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Expression::_Add(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	int m = 0;
	int n = 0;
	string str1 = obj.first.getValue();
	string str2 = obj.second.getValue();
	m = atoi(str1.c_str());
	n = atoi(str2.c_str());
	return m + n;
}

int Expression::_Sub(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	int m = 0;
	int n = 0;
	string str1 = obj.first.getValue();
	string str2 = obj.second.getValue();
	m = atoi(str1.c_str());
	n = atoi(str2.c_str());
	return n - m;
}

int Expression::_Mul(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	int m = 0;
	int n = 0;
	string str1 = obj.first.getValue();
	string str2 = obj.second.getValue();
	m = atoi(str1.c_str());
	n = atoi(str2.c_str());
	return m * n;
}

int Expression::_Div(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	int m = 0;
	int n = 0;
	string str1 = obj.first.getValue();
	string str2 = obj.second.getValue();
	m = atoi(str1.c_str());
	n = atoi(str2.c_str());
	return n / m;
}

int Expression::_And(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	int m = 0;
	int n = 0;
	string str1 = obj.first.getValue();
	string str2 = obj.second.getValue();
	m = atoi(str1.c_str());
	n = atoi(str2.c_str());
	if (m == 0 || n == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int Expression::_Or(UnionOperand a, UnionOperand b)
{
	pair<UnionOperand, UnionOperand> p1(a, b);
	pair<UnionOperand, UnionOperand> obj = typeCast(p1);
	int m = 0;
	int n = 0;
	string str1 = obj.first.getValue();
	string str2 = obj.second.getValue();
	m = atoi(str1.c_str());
	n = atoi(str2.c_str());
	if (m != 0 || n != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Expression::_Not(UnionOperand a)
{
	int m = 0;
	string str = a.getValue();
	m = atoi(str.c_str());
	return 1 - m;
}