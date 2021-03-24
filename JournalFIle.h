#include "common.h"

class JournalFile
{
private:
    string jfName;//日志文件名
    string jfPath;//日志文件路径

public:
    bool writeLog();//写日志(该函数有参数,未定),写回成功返回true
};