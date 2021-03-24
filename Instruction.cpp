#include "common.h"
#include "Instruction.h"

Instruction::Instruction(OpCode opCode, string p1, string p2, string p3)
{
	this->opCode = opCode;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}

Instruction::Instruction()
{

}