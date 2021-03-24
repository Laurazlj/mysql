#pragma once
#ifndef	 _EvalDiscription_
#define  _EvalDiscription_
#include "Instruction.h"

class EvalDiscription 
{
public:
	OpCode cmd;
	string col_name;
	string constant;
	EvalDiscription(OpCode pcmd, string pcolname, string pconstant);
	EvalDiscription(OpCode pcmd);
	EvalDiscription();
};

#endif