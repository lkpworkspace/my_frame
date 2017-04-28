#include <iostream>
#include "MyFrame.h"
#include "MyUdp.h"
#include "MySqlite3.h"
#include "MyTalkServer.h"

using namespace std;
using namespace my_master;
MyUdp *g_udp = nullptr;
MySqlite3* g_db = nullptr;

int main(int argc, char *argv[])
{
    argc = argc;
    argv = argv;
    // init my database
    g_db = new MySqlite3("mytalk.db");
    g_db->Open();
#if 0 // create table
    std::string sql;
    sql = "CREATE TABLE member(account TEXT NOT NULL, name TEXT NOT NULL, password TEXT NOT NULL, mark TEXT)";
    g_db->ExecSql(sql.c_str());
    sql = "CREATE TABLE friend(account TEXT NOT NULL, belong TEXT, friend TEXT NOT NULL)";
    g_db->ExecSql(sql.c_str());
#endif
#if 0 // insert data
    std::string sql;
    sql = "insert into member values('kpli','kpli','123456','easy coding on linux')";
    g_db->ExecSql(sql.c_str());
#endif
#if 0
    g_db->Close();
#endif
    // init my frame
    MyApp app(2,1024);

    g_udp = new MyUdp("",4399,true);
    g_udp->Bind();
    app.AddEvent(g_udp);

    // init my talk server
    MyTalkServer* server = new MyTalkServer();

    // run
    return app.Exec();
}
