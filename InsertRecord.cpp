#include "common.h"
#include "InsertRecord.h"

InsertRecord::InsertRecord(string pcolname, string ptype, string pvalue)
{
	this->colname = pcolname;
	this->type = ptype;
	this->value = pvalue;
}

InsertRecord::InsertRecord()
{

}

string InsertRecord::getcolname()
{
	return this->colname;
}

string InsertRecord::getvalue()
{
	return this->value;
}