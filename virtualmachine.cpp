#include "vm.h"
#include "change.h"
#include <cmath>
#include <iostream>

DataBase* db;
vector<Instruction> instructions;
list<Column> columns;
list<pair<Type, string>> tablecol;
string targetTable;
string pkName;
string dbname;
Activity activity;
list<InsertRecord> record;
vector<vector<EvalDiscription>> updatevalues;
vector<EvalDiscription> updatevalue;
vector<EvalDiscription> filters;
list<string> selcolumns;
list<string> updateAttrs;
string constant;
int cnt = 0;
int number = 0;
int joinIndex = 0;
bool isJoin = false;

bool equals(Column col1, Column col2)
{
	if (col1.getColName() == col2.getColName())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void run(Instruction instruction)
{
	OpCode opcode;
	OpCode temp;
	string p1;
	string p2;
	string p3;
	opcode = instruction.opCode;
	p1 = instruction.p1;
	p2 = instruction.p2;
	p3 = instruction.p3;
	Type type;
	Column col;
	InsertRecord rec;
	EvalDiscription eva;
	if (opcode == 26 || opcode == 31)
	{
		if (p2 == "int")
		{
			type.setToInt();
		}
		else if (p2 == "char")
		{
			type.setToChar();
		}
		else
		{
			string str1 = p2.substr(4);
			number = atoi(str1.c_str());
			type.setToString(number);
		}
	}
	switch (opcode)
	{
	case UseDB:
		targetTable = p1;
		UseDatabase(targetTable);
		break;
	case DropDB:
		activity = Dropdb;
		targetTable = p1;
		break;
	case showDB:
		cout << "Show database" << endl;
		break;
	case showTB:
		showtable();
		break;
	case createDB:
		activity = Createdb;
		dbname = p1;
		break;
	case Createtb:
		targetTable = p3;
		activity = Createtable;
		columns.clear();
		break;
	case Addcol:
		col = Column(type, p1, cnt);
		cnt++;
		columns.push_back(col);		
		break;
	case BeginPK:
		break;
	case AddPK:
		pkName = p1;
		break;
	case EndPK:
		break;
	case Droptb:
		targetTable = p3;
		activity = Droptable;
		break;
	case Insert:
		targetTable = p3;
		activity = Ins;
		record.clear();
		updatevalues.clear();
		break;
	case Delete:
		targetTable = p3;
		activity = Del;
		break;
	case Select:
		targetTable = p3;
		activity = Sel;
		break;
	case Update:
		targetTable = p3;
		activity = Upd;
		break;
	case BeginItem:
		break;
	case AddItemCol:
		rec = InsertRecord(p1, p2, p3);
		record.push_back(rec);
		break;
	case EndItem:
		break;
	case BeginFilter:
		break;
	case EndFilter:
		filters = updatevalue;
		break;
	case BeginColSelect:
		break;
	case AddColSelect:
		selcolumns.push_back(p1);
		break;
	case SelectStar:
	    selectstar();
	    break;
	case EndColselect:
		break;
	case BeginJoin:
		isJoin = true;
		joinIndex = 0;
		break;
	case AddTable:
		targetTable = p1;
		join(targetTable);
		break;
	case EndJoin:
		break;
	case Set:
		updateAttrs.push_back(p1);
		constant = p2;
		break;
	case BeginExpression:
		updatevalue.clear();
		break;
	case EndExpression:
		updatevalues.push_back(updatevalue);
		break;
	case Operand:
		eva = EvalDiscription(opcode, p2, p3);
		updatevalue.push_back(eva);
		break;
	case Operator:
		temp = operator_to_OpCode(p2);
		eva = EvalDiscription(temp);
		updatevalue.push_back(eva);
		break;
	case Execute:
		execute();
		break;
	default:
		activity = Qu;
		break;
	}
}

void ConditonClear()
{
	tablecol.clear();
	instructions.clear();
	filters.clear();
	selcolumns.clear();
	columns.clear();
	record.clear();
	updatevalues.clear();
	updatevalue.clear();
	updateAttrs.clear();
	targetTable = "";
	pkName = "";
	dbname = "";
	constant = "";
	cnt = 0;
	number = 0;
	joinIndex = 0;
	isJoin = false;
}

void join(string tableName)
{

}

void execute()
{
	switch (activity)
	{
	case Ins:
		insert();
		updatevalues.clear();
		break;
	case Del:
		_delete();
		break;
	case Upd:
		update();
		break;
	case Sel:
		select();
		isJoin = false;
		break;
	case Createtable:
		CreateTable();
		break;
	case Droptable:
		dropTable();
		break;
	case Createdb:
		CreateDatabase();
		break;
	case Dropdb:
		DropDatabase();
		break;
	case Qu:
		QuitDatabase();
		break;
	default:
		break;
	}
}

void selectstar()
{
	Table tab = db->getTable(targetTable);
	selcolumns = tab.getColumnsName();
}

BasicType BType(string strVal)
{
	int len = strVal.length();
	if (len == 1)
	{
		char c;
		c = strVal.at(0);
		if (c > '9'&&c < '0')
		{
			return CHARTYPE;
		}
	}
	else if (len > 1)
	{
		char c;
		c = strVal.at(0);
		if (c <= '9'&&c >= '0')
		{
			return INTTYPE;
		}
		else
		{
			return STRINGTYPE;
		}
	}
	return UNKOWNTYPE;
}

UnionOperand eval(vector<EvalDiscription> evalDiscriptions, TableCursor p)
{
	Expression exp;
	Table tab = db->getTable(targetTable);
	list<string> info= tab.getColumnsName();
	list<string>::iterator itor1;
	for (int i = 0; i < evalDiscriptions.size(); i++) 
	{
		if (evalDiscriptions[i].cmd == Operand)
		{
			if (evalDiscriptions[i].col_name != "")
			{
				for (itor1 = info.begin() ; itor1 != info.end() ; itor1++)
				{
					if ((*itor1) == (evalDiscriptions[i].col_name))
					{
						BasicType type = tab.getColumn(evalDiscriptions[i].col_name).getColType().getType();
						int num = tab.getColumn(evalDiscriptions[i].col_name).getColNum();
						exp.addOperand(UnionOperand(type, p.getData()[num]));
					}
				}

			}
			else 
			{
				string val = evalDiscriptions[i].constant;
				BasicType cType = BType(val);
				exp.addOperand(UnionOperand(cType, val));
			}
		}
		else 
		{
			exp.applyOperator(evalDiscriptions[i].cmd);
		}
	}
	return exp.getAns();
}

bool check(TableCursor p)
{
	if (filters.size() == 0)
	{
		return true;
	}
	UnionOperand ans;
	ans = eval(filters, p);
	if (ans.getType() == STRINGTYPE)
	{
		return false;
	}
	else if (abs(atoi(ans.getValue().c_str())) < 1e-10)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void UseDatabase(string target)
{
	db = new DataBase(target);
}

void showtable()
{
	Table tab = db->getTable("master");
	TableCursor p = db->getTable("master").createCursor();
	list<string> header = tab.getColumnsName();
	list<string>::iterator itor;
	while (1)
	{
		vector<string> ans;
		int j = 0;
		for (j = 0, itor = header.begin(); j < header.size() && itor != header.end(); j++, itor++)
		{
			ans.push_back(p.getData()[j]);
		}
		for (int i = 0; i < ans.size(); i++)
		{
			cout << ans[i] << " ";
		}
		cout << endl;
		if (false == p.moveToNext())
		{
			break;
		}
	}
}

void select()
{
	vector<string> selected;
	list<string> tmp;
	list<string>::iterator itor;
	for (itor = selcolumns.begin(); itor != selcolumns.end(); itor++)
	{
		string str = *itor;
		selected.push_back(str);
	}
	TableCursor p = db->getTable(targetTable).createCursor();
	p.moveToSon();
	Table tab = db->getTable(targetTable);
	list<string> header = tab.getColumnsName();
	cout << "select result:" << endl;
	while (1)
	{
		if (check(p))
		{
			vector<string> ans;
			int j = 0;
			for (j = 0 , itor = header.begin(); j < header.size() && itor != header.end(); j++ , itor++)
			{
				for (int i = 0; i < selected.size(); i++)
				{
					if ((*itor) == selected[i])
					{
						ans.push_back(p.getData()[j]);
					}
				}
			}
			for (int i = 0; i < ans.size(); i++)
			{
				cout << ans[i] << " ";
			}
			cout << endl;
		}
		if (false == p.moveToNext())
		{
			break;
		}
	}
	activity = None;
}

void _delete()
{
	TableCursor p = db->getTable(targetTable).createCursor();
	p.moveToSon();
	while (1)
	{
		if (check(p))
		{
			if(false == p.deleteVal())
			{
				break;
			}
		}
		else
		{
			if (false == p.moveToNext())
			{
				break;
			}
		}
	}
	activity = None;
}

void update()
{
	TableCursor p = db->getTable(targetTable).createCursor();
	p.moveToSon();
	Table tab = db->getTable(targetTable);
	list<string> header = tab.getColumnsName();
	while (1)
	{
		vector<string> row = p.getData();
		if (check(p))
		{
			list<string>::iterator itor;
			for (itor = updateAttrs.begin(); itor != updateAttrs.end(); itor++)
			{
				string attrname = *itor;
				list<string>::iterator itor1;
				int j = 0;
				for (j = 0, itor1 = header.begin(); j < header.size() && itor1 != header.end(); j++, itor1++)
				{
					if ((*itor1) == attrname)
					{
						row[j] = constant;
					}
				}
			}
		}
		list<string> row1;
		for (int i = 0; i < row.size(); i++)
		{
			row1.push_back(row[i]);
		}
		p.setData(row1);
		if (false == p.moveToNext())
		{
			break;
		}
	}
	activity = None;
}

void insert()
{
	list<string> colvalues;
	list<InsertRecord>::iterator itor;
	for (itor = record.begin(); itor != record.end(); itor++)
	{
		string value;
		value = itor->getvalue();
		colvalues.push_back(value);
	}
	db->getTable(targetTable).createCursor().insert(colvalues);
	activity = None;
}

void CreateTable()
{
	list<Column>::iterator itor;
	itor = columns.begin();
	pair<Type, string> coldef;
	Type type;
	string str;
	pkName = itor->getColName();
	while (itor != columns.end())
	{	
		type = itor->getColType();
		str = itor->getColName();
		coldef = make_pair(type, str);
		tablecol.push_back(coldef);
		itor++;
	}	
	db->createTable(targetTable, pkName, tablecol);
	activity = None;
}

void dropTable()
{
	db->dropTable(targetTable);
	activity = None;
}

void CreateDatabase()
{
	db = new DataBase(dbname);
	db->close();
	activity = None;
}

void DropDatabase()
{
	db->dropDatabase(targetTable);
	activity = None;
}

void QuitDatabase()
{
	db->close();
	activity = None;
}

void virtualmachine(vector<FourVarCode> fourVarCodeTab)
{		
	OpCode opcode;
	string p1;
	string p2;
	string p3;
	generatemap();
	for (int i = 0; i < fourVarCodeTab.size(); i++)
	{
		string op;
		op = fourVarCodeTab[i].op;
		opcode = string_to_OpCode(op);
		if (opcode == 0)
		{
			exit(0);
		}
		p1 = fourVarCodeTab[i].num_a;
		p2 = fourVarCodeTab[i].num_b;
		p3 = fourVarCodeTab[i].num_c;
		Instruction instruction= Instruction(opcode, p1, p2, p3);
		instructions.push_back(instruction);
	}
	for (int i = 0; i < instructions.size(); i++)
	{
		Instruction instruction = instructions[i];
		run(instruction);
	}
	ConditonClear();
}
