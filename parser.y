%{
#include "common.h"
#include "GenByteCode.h"
#include "vm.h"

using namespace std;

extern int yylex(void);
void yyerror(string s)
{
	extern char *yytext;
	int len=strlen(yytext);
	int i;
	char buf[512]={0};
	for (i=0;i<len;++i)
	{
		sprintf(buf,"%s%c",buf,yytext[i]);
	}
	fprintf(stderr, "ERROR: %s at symbol '%s'\n", s.c_str(), buf);
}
int fieldall=0;
string s,num,opstring,dbName,tablename,fieldname,typestring,setdata;
string ftmp, ttmp;//field,type
//vector<pair<string,string>> fdef;//fieldsdefinition
bool star=0;
vector<pair<string,string>> fild_value;
vector<fieldtype> fdef;
map<string, vector<fieldtype> >table_field;//table_fielddefination
vector<string>datavalue;
vector<string>selectfiedv;
vector<fieldtype>insertfv;
map<string,string>updatef_dv;
vector<string>jointable;

extern char* tokenctx; 
%}

%left OR
%left AND
%right NOT
%nonassoc '=' '>' '<'
%left '+' '-'
%left '*' '/'
%token  CREATE TABLE CHAR INT  DATABASE USE SHOW DATABASES  
%token EXIT QUIT DROP TABLES INSERT INTO VALUES 
%token SELECT FROM WHERE FALSE TRUE AND NOT OR  DELETE UPDATE SET 
%token ID NUMBER STRING PRIMARY KEY
%%
statements : statement{return 0;};
statement : usedatabasesql
			| showsql 
			| createsql 
			| dropsql 
			| insertsql 
			| selectsql 
			| deletesql 
			| updatesql 
			| exitsql ;
usedatabasesql : USE databasename';'{useDatabase(dbName);};
showsql : SHOW DATABASES ';' {showDatabases();}
			| SHOW TABLES ';' {showTables();};
databasename : ID {dbName = tokenctx;};
createsql : CREATE DATABASE databasename ';' {createDatabase(dbName);table_field.clear();}
			| CREATE TABLE table {createtable(tablename);} '('{fdef.clear();} fieldsdefinition ')' ';'{for(int i=0;i<fdef.size();i++) table_field[tablename].push_back(fdef[i]);
		};
dropsql : DROP DATABASE databasename ';'{dropDB(dbName);} | DROP TABLE table ';'{droptb(tablename);};
insertsql : INSERT INTO table '(' fields ')' VALUES '(' 
			{datavalue.clear();}datas ')' ';' 
			{inserttablename(tablename);
			insertintotable(tablename,insertfv,datavalue);
			insertfv.clear();
			} 
			| INSERT INTO table VALUES {datavalue.clear();}'(' datas ')' ';'
			{
				inserttablename(tablename);
				insertfv.clear();
				for(int i=0;i<table_field[tablename].size();i++)
					{
						insertfv.push_back(table_field[tablename][i]);
					}
				insertintotable(tablename,insertfv,datavalue);
				insertfv.clear();
			};			

selectsql : SELECT fields_star FROM tables ';'
			{
				if(star==true)
				{
					selectstar(tablename);
					star=0;
				}
				else
				{
					selectfield(tablename,selectfiedv);
					selectfiedv.clear();
				
				}
				
			} 
			| SELECT fields_star FROM tables WHERE 
			{
				if(star==true)
				{
					selectstar(tablename);
					star=0;
				}
				else
				{
					selectfield(tablename,selectfiedv);
					selectfiedv.clear();
					
				}
				
				beginfilter();
			}conditions
			{endfilter();} ';' ;


deletesql : DELETE FROM table WHERE{deleteRol(tablename);beginfilter();}conditions{endfilter();} ';'| DELETE FROM table ';'{deleteRol(tablename);};


updatesql : UPDATE table SET fields_values ';'
			{
				updatetable(tablename);
				setcolname(updatef_dv);
				updatef_dv.clear();

			}
			| UPDATE table SET fields_values 
			WHERE 
			{
				updatetable(tablename);
				setcolname(updatef_dv);
				beginfilter();
				updatef_dv.clear();
			} 
			conditions
			{endfilter();} ';';
fields_values    : field '=' data{updatef_dv[fieldname]=setdata;}',' fields_values 
				|  field '=' data{updatef_dv[fieldname]=setdata;};

tables : table{jointable.push_back(tablename);} ',' tables | table {jointable.push_back(tablename);};
fields_star : selectfields | '*'{star=TRUE;}; 
selectfields : field{selectfiedv.push_back(fieldname);} ',' selectfields | field{selectfiedv.push_back(fieldname);}  ;
conditions : '(' conditions ')' 
				| conditions AND conditions {insertoperator("AND");}
				| conditions OR conditions {insertoperator("OR");}
				| NOT conditions {insertoperator("NOT");}
				| condition '<' '>' condition {opstring="<>";insertoperator(opstring);}
				| condition '!' '=' condition {opstring="!=";insertoperator(opstring);}
				| condition '>' condition {opstring=">";insertoperator(opstring);}
				| condition '>' '=' condition {opstring=">=";insertoperator(opstring);}
				| condition '<' condition {opstring="<";insertoperator(opstring);}
				| condition '<' '=' condition {opstring="<=";insertoperator(opstring);}
				| condition  '=' condition {opstring="=";insertoperator(opstring);}
				| TRUE{opstring="TRUE";insertoperator(opstring);} 
				| FALSE{opstring="FALSE";insertoperator(opstring);} ;
condition : expr | field{insertoprand_col(fieldname);} | string{insertoprand(s);};
expr   : expr '+' expr {opstring="+";insertoperator(opstring);}
		| expr '-' expr{opstring="-";insertoperator(opstring);}
		| expr '*' expr {opstring="*";insertoperator(opstring);}
		| expr '/' expr {opstring="/";insertoperator(opstring);}
		| NUMBER{num=tokenctx;insertoprand(num);} ;
fields : field{
			fieldtype ft;
			ft.fname=fieldname;
			for(int i=0;i<table_field[tablename].size();i++)
			{
				if(table_field[tablename][i].fname==fieldname)
				{
					ft.ftype=table_field[tablename][i].ftype;
				}
			}
			insertfv.push_back(ft);
			} ',' fields 
		| field{
			fieldtype ft;
			ft.fname=fieldname;
			for(int i=0;i<table_field[tablename].size();i++)
			{
				if(table_field[tablename][i].fname==fieldname)
				{
					ft.ftype=table_field[tablename][i].ftype;
				}
			}
			insertfv.push_back(ft);	
		};
datas :   data |   data ',' datas;
data : string{datavalue.push_back(tokenctx);setdata=tokenctx;} | NUMBER{datavalue.push_back(tokenctx);setdata=tokenctx;} ;
string :   '\'' STRING {s=tokenctx;}'\''| '\'' ID {s=tokenctx;}'\''| '\'' NUMBER{s=tokenctx;} '\'';			
table : ID{tablename = tokenctx;};
fieldsdefinition : field_type {AddCol(fieldname,typestring);fieldtype ft;ft.fname=fieldname;ft.ftype=typestring;fdef.push_back(ft);}
				|  field_type  ','{AddCol(fieldname,typestring);fieldtype ft;ft.fname=fieldname;ft.ftype=typestring;fdef.push_back(ft);} fieldsdefinition;
field_type : field  type 
			| field type PRIMARY{pk(fieldname);}KEY; 
field : ID{fieldname = tokenctx;} ;

type : CHAR{ typestring = "char";} | CHAR '('NUMBER{num=tokenctx;}')'{typestring = "char"+num;}| INT{typestring="int";} ;
exitsql : EXIT{exit();} | QUIT{quit();};
%%

int main(void)
{
	while(1){
		printf("mysql> ");
		fourVarCodeTab.clear();
		yyparse();
    	insertIntoFourVarCodeTab("Execute","","","");

		printmidcode();
		//cout<<""<<endl;
		virtualmachine(fourVarCodeTab);
		
	}
	return 0;
}
