#include "MyMsgCommon.h"


static MyMsgServer* g_server = NULL;
static MySqlite3* g_db = NULL;

void SetMsgServer(MyMsgServer* s)
{
    g_server = s;
}

MyMsgServer* GetMsgServer()
{
    return g_server;
}

void SetMySqlite3(MySqlite3* s)
{
    g_db = s;
}

MySqlite3* GetMySqlite3()
{
    return g_db;
}

unsigned char CheckSum(const unsigned char* buf, int len)
{
    unsigned char num = 0;
    for(int i = 0; i < len; ++i)
    {
        num += buf[i];
    }
    return num;
}

std::string GetAccount()
{
    std::string temp;
    char buf[9] = {0};
again:
    temp.clear();
    memset(buf,0,sizeof(buf));
    for(int i = 0; i < 9; ++i)
    {
        buf[i] = MyHelp::RandomNum(0,10);
        temp += MyHelp::ToChar(buf[i]);
    }
    //    if(REQUEST(temp) != NULL)
    //        goto again;
    return temp;
}

std::string Getpass()
{
    std::string pass;
    char buf[4] = {0};

    for(int i = 0; i < 4; ++i)
    {
        buf[i] = MyHelp::RandomNum(0,10);
        pass += MyHelp::ToChar(buf[i]);
    }
    return pass;
}
