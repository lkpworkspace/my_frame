#include <iostream>
#include "MyMsgCommon.h"
#include "MyTcp.h"
#include "MyMsgServer.h"
#include "MyMsgManager.h"

using namespace std;

int main(int argc, char *argv[])
{
    UNUSE_ARG(argc,argv);
    // as a daemon
    MyHelp::DaemonInit();

    MyApp app(4);

#if 0 // test MyMsgManager
    sockaddr_in addr;
    MyMsgConnect* conn = new MyMsgConnect(10,addr);
    conn->m_id = "110";
    conn->m_group = "kpli";
    conn->m_server = "kpli";
    MyMsgManager::GetInstance()->InsertConnect(conn);
    MyMsgConnect* c = MyMsgManager::GetInstance()->GetConnect("kpli","kpli","110");
    std::cout << c->m_id;
#endif
    // init database
    MySqlite3 db("../dat/MyMsgDB.dat");
    db.Open();
    SetMySqlite3(&db);

    // init tcp server
    MyMsgServer *server = new MyMsgServer();
    server->SetReuseSock();
    server->Bind();
    server->Listen();
    server->SetNonblock(true);
    app.AddEvent(server);

    SetMsgServer(server);

    return app.Exec();
}
