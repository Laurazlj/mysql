#include "EvalDiscription.h"

EvalDiscription::EvalDiscription(OpCode pcmd, string pcolname, string pconstant)
{
	this->cmd = pcmd;
	this->col_name = pcolname;
	this->constant = pconstant;
}

EvalDiscription::EvalDiscription(OpCode pcmd)
{
	this->cmd = pcmd;
}

EvalDiscription::EvalDiscription()
{

}