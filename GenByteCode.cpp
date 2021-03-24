#include "GenByteCode.h"
vector<FourVarCode> fourVarCodeTab;

int varnum=0;

void insertIntoFourVarCodeTab(string op, string a, string b, string c)
{
   

	int length = fourVarCodeTab.size();

	if (length>=2048) {
		fprintf(stderr, "out of four var table index!\n");
		exit(1);
	}
    FourVarCode f;
    f.op=op;
    f.num_a=a;
    f.num_b=b;
    f.num_c=c;
    fourVarCodeTab.push_back(f);
    
}

vector<FourVarCode> getMidCode()
{
	return fourVarCodeTab;
}
void printmidcode()
{
    // FILE*f=fopen("result/midcode.txt","w");
    // char*tmp=(char*)malloc(sizeof(char)*128);
    // cout<<"print IR"<<endl;
    for(int i=0;i<fourVarCodeTab.size();i++)
    {
       cout<<fourVarCodeTab[i].op.c_str()<<","
       <<fourVarCodeTab[i].num_a.c_str()<<","
        <<fourVarCodeTab[i].num_b.c_str()<<","
       << fourVarCodeTab[i].num_c.c_str()<<endl;
        // fwrite(tmp,sizeof(char),strlen(tmp),f);
    }
    // fclose(f);
}
string generateVar()
{
    char *var=(char*)malloc(sizeof(char)*16);
    sprintf(var,"$_%d",varnum);
    varnum++;
    return var;
}
void selectstar(string tablename)
{
    insertIntoFourVarCodeTab("Select","","",tablename);
    insertIntoFourVarCodeTab("BeginColSelect","","","");
    insertIntoFourVarCodeTab("SelectStar","","","");
    insertIntoFourVarCodeTab("EndColselect","","","");
    

}
void useDatabase(string dbName)
{   
    insertIntoFourVarCodeTab("UseDB",dbName,"","");
    //printf("use %s\n", dbName.c_str());
}

void showDatabases()
{
    insertIntoFourVarCodeTab("showDB","","","");
    //printf("show databases\n");
}

void showTables()
{
    insertIntoFourVarCodeTab("showTB","","","");
   // printf("show tables\n");
}

void createDatabase(string dbName)
{
    insertIntoFourVarCodeTab("createDB",dbName,"","");
   // printf("create database %s\n", dbName.c_str());
}

void createtable(string tablename)
{
    insertIntoFourVarCodeTab("Createtb","","",tablename);

}
void AddCol(string fieldname,string typestring)
{
    insertIntoFourVarCodeTab("Addcol",fieldname,typestring,"");
}
void dropDB(string dbname)
{
    insertIntoFourVarCodeTab("DropDB",dbname,"","");

}
void droptb(string tablename)
{
    insertIntoFourVarCodeTab("Droptb","","",tablename);

}
//insert
void inserttablename(string tablename)
{
    // cout<<"INSERT =========="<<endl;
    insertIntoFourVarCodeTab("Insert","","",tablename);
    //cout<<"INSERT =========="<<endl;

}
void insertintotable(string tablename,vector<fieldtype> fieldv,vector<string>datav)
{
    insertIntoFourVarCodeTab("BeginItem","","","");
   
    if(fieldv.size()==datav.size())
    {
        for(int i=0;i<datav.size();i++)
        {
           // cout<<"type::::::::"<<fieldv[i].ftype<<endl;
        insertIntoFourVarCodeTab("AddItemCol",fieldv[i].fname,fieldv[i].ftype,datav[i]);
        }

    }
    else
    {
        cout<<"field and data's size are not metch"<<endl;
    }
    

    insertIntoFourVarCodeTab("EndItem","","","");

}




//select
void selectfield(string tablename,vector<string> selectfiedv)
{
    insertIntoFourVarCodeTab("Select","","",tablename);
    insertIntoFourVarCodeTab("BeginColSelect","","","");
    for(int i=0;i<selectfiedv.size();i++)
    {
        insertIntoFourVarCodeTab("AddColSelect",selectfiedv[i],"","");
    }
    insertIntoFourVarCodeTab("EndColselect","","","");
}
//join
string IRjointable(vector<string>jointable)
{
    //join逻辑： 如果只有一张表 直接在tablename中 select
    //如果有很多张表 先将这些表链接 成为$x  在$x中进行select
    insertIntoFourVarCodeTab("BeginJoin","","","");
    for(int i=0;i<jointable.size();i++)
    {
        insertIntoFourVarCodeTab("AddTable",jointable[i],"","");
    }
    string jointablename=generateVar();
    insertIntoFourVarCodeTab("EndJoin",jointablename,"","");
    return jointablename;
}
//condition
void insertoprand(string constant)
{
    insertIntoFourVarCodeTab("Operand","","",constant);

}

void insertoperator(string IRoperator)
{
    insertIntoFourVarCodeTab("Operator","",IRoperator,"");

}
void insertoprand_col(string col_name)
{
    insertIntoFourVarCodeTab("Operand","",col_name,"");

}
void beginfilter()
{
    insertIntoFourVarCodeTab("BeginFilter","","","");

}
void endfilter()
{
    insertIntoFourVarCodeTab("EndFilter","","","");

}
//delete删除行
void deleteRol(string tablename)
{
    insertIntoFourVarCodeTab("Delete","","",tablename);
}
//update
void updatetable(string tablename)
{
    insertIntoFourVarCodeTab("Update","","",tablename);
}
//set
void setcolname(map<string,string> updatef_dv)
{
    //对map进行遍历  
    for(auto i=updatef_dv.begin();i!=updatef_dv.end();++i)
    {
    insertIntoFourVarCodeTab("Set",i->first,i->second,"");

    }

}
//exit
void exit()
{
    insertIntoFourVarCodeTab("Exit","","","");
    // cout<<"exit"<<endl;
    // cout<<"fourVarCodeTablength: "<<fourVarCodeTab.length<<endl;
}

void quit()
{
     insertIntoFourVarCodeTab("Quit","","","");
}
void pk(string colname)
{
     insertIntoFourVarCodeTab("BeginPK","","","");
     insertIntoFourVarCodeTab("AddPK",colname,"","");
     insertIntoFourVarCodeTab("EndPK","","","");

}
