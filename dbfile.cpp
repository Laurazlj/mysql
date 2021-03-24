#include "dbfile.h"

fstream file;
FILE *ffp;
int lastid;
header writebackheader;
header readheader;
header::header(){}
string NAME;
BYTE* FindPageFromFile(int pageId)
{
    //file.open("./dbFile/q");
    //通过page 的id 推算page index
    BYTE* data = (BYTE*)malloc(PAGESIZE * sizeof(unsigned char));//大小为PAGESIZE * sizeof(unsigned char)字节
    cout<<pageId<<endl;
    if(pageId>=lastid)
    {
        //先全写上0
        //cout<<"-----------写0-------"<<endl;  
        char cc=0; 
        fseek(ffp,(pageId+1)*PAGESIZE*sizeof(unsigned char),0);
        //long pos=ftell(ffp);
        //cout<<"---writepos----"<<pos<<endl;
        fwrite(&cc,sizeof(char),PAGESIZE,ffp);
        fflush(ffp);
       
        fseek(ffp,(pageId+1)*PAGESIZE*sizeof(unsigned char),0);
        //pos=ftell(ffp);
        //cout<<"---readpos-----"<<pos<<endl;确保了读写指针的位置是一样的
        fread(data,PAGESIZE*sizeof(unsigned char),1,ffp);
        //printPCharByTwo_(data);

    }
    else{
        fseek(ffp,(pageId+1)*PAGESIZE*sizeof(unsigned char),0);
        fread(data,PAGESIZE*sizeof(unsigned char),1,ffp);
    }
    //用ffp读取
    
    //cout<<"data :: "<<data<<endl;
    return data;


}

void printPCharByTwo_(BYTE* pchr)
{
    //printf("data01 : ");
    for(int i=0;i<PAGESIZE;i++)
    {
        for(int j=7; j>=0; j--)
        {
            unsigned char tmpp = pchr[i];
            tmpp=pchr[i]&(1<<j);
            printf("%d", tmpp>>j);
        }
        printf(" ");
    }
    printf("\n");
}

void WriteBack(int pageId, BYTE* pagecontent)
{   
   if(pageId>=lastid)
   {
       //更新lastid
       
       lastid=pageId+1;
   }
   
    //使用ffp指针
    fseek(ffp,(pageId+1)*PAGESIZE*sizeof(unsigned char),0);
    fwrite(pagecontent,PAGESIZE*sizeof(unsigned char),1,ffp);

}
void WriteBacktest(int pageId)
{
   if(pageId>=lastid)
   {
       //更新lastid
       
       lastid=pageId+1;
   }
    
}
bool CreateDataBase(string name,string path)
{
    NAME=name;
    //创建文件，文件名为name  路径为path
    ofstream createfile;
    lastid=0;
    string f=path+"/"+name;// is path have"\\"???
    createfile.open(f);
    header fileheader;
    fileheader.lastid=0;
    bool flag;
    if(createfile)
    {
        
        cout<<"create successful"<<endl;
        flag= true;
    } 
    else
    {
        flag= false;
        cout<<"create database failed"<<endl;
    }
    createfile.close();
    
    
    // 写header部分   header大小为pagesize   
    writeheader(f,fileheader);
    //使用指针打开文件
    ffp=fopen(f.c_str(),"r+");//rb+ r+以读写方式打开
    
   

    // for(int i=-1;i<5;i++)
    // {
    //     BYTE* test;
    //     test=FindPageFromFile(i);
    //     printPCharByTwo_(test);
    // }
    //cout<<flag<<endl;
    return flag;

}
void closeDataBase(string name,string path)
{
    //file.close();
    //写回
    fclose(ffp);
    ffp = NULL;
    string f=path+"/"+name;// is path have"\\"???
    writebackheader.lastid=lastid;
    writeheader(f,writebackheader);

    
}
void writeheader(string allpath,header thisheader)
{
    string f=allpath;
    FILE *fp;
    fp=fopen(f.c_str(),"r+");
    int tmpoffset=0;
    fseek(fp,tmpoffset,0);
    fwrite(&thisheader.lastid,sizeof(int),1,fp);
    // tmpoffset=tmpoffset+sizeof(int);
    // fseek(fp,tmpoffset,0);
    int tmpsize=thisheader.FIlereleasedPageID.size();
    int tmpid;
    fwrite(&tmpsize,sizeof(int),1,fp);
    // tmpoffset=tmpoffset+sizeof(int);
    // fseek(fp,tmpoffset,0);
    list<int>::iterator it;
    
    for(it=thisheader.FIlereleasedPageID.begin();it!=thisheader.FIlereleasedPageID.end();it++)
    {
        tmpid=*it;
        fwrite(&tmpid,sizeof(int),1,fp);
        // tmpoffset=tmpoffset+sizeof(int);
        // fseek(fp,tmpoffset,0);
    }

    //fwrite(&thisheader,sizeof(header),1,fp);

    fclose(fp);
    fp = NULL;
}
bool DeleteDB(string path,string DBname)
{
    //删除数据库，直接删除文件  
    char str[512];
    string f=path+"/"+DBname;
    //strcpy(str,path);
    //str=f.c_str();
    int i;
    for( i=0;i<f.length();i++)
    {
        str[i]=f[i];
    }
    str[i]='\0';
    int flag=remove(str);
    cout<<"delete successful"<<endl;
    return flag;
}
int lastID()
{
     return lastid;
}
bool OpenDB(string path,string dbname)
{
    NAME=dbname;

    string f=path+"/"+dbname;// is path have"\\"???
    
    bool flag;
    FILE *fp;
    fp=fopen(f.c_str(),"r+");

    fread(&lastid,sizeof(int),1,fp);;
    int tmpsize;
    fread(&tmpsize,sizeof(int),1,fp);
    
    int tmpid;
    for(int i=0;i<tmpsize;i++)
    {
        fread(&tmpid,sizeof(int),1,fp);
        readheader.FIlereleasedPageID.push_back(tmpid);
    }

   
    fclose(fp);
    fp = NULL;
    //fopen(f,"r+")
    
    // file.open(f);
    // if(file.is_open())
    // {
    //     //TODO:读取lastid和list
    //    // fread(&thisheader,sizeof(header),1,file);
    //     flag= true;
    // }
    // else
    // {
    //     flag= false;
    // }
    ffp=fopen(f.c_str(),"r+");
    if(ffp!=NULL)
    {
        flag=true;
    }
    else{flag=false;}
    return flag;

   
}
list<int> FIleGetReleasedID()
{
    return readheader.FIlereleasedPageID;
    
}
void FIleWriteUnusedID(list<int>ReID)
{
    //Todo: 写回lastid和released-list
   
    writebackheader.lastid=lastid;
    int tmp;
    list<int>::iterator it;
    for(it=ReID.begin();it!=ReID.end();it++)
    {
        tmp=*it;
        writebackheader.FIlereleasedPageID.push_back(tmp);
    }

    
}
bool FileNotExist(string dbname)
{
    //文件不存在返回true
    string p=DBFOLD;
    string f=p+"/"+dbname;
    ifstream ifexist(f.c_str());
    bool flag=!ifexist.good();
    return flag;
}