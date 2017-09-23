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
