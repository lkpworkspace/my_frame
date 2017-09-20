#include <iostream>
#include "MyFrame.h"
#include "MyTcp.h"
#include "MyMsgServer.h"

using namespace std;

int main(int argc, char *argv[])
{
    UNUSE_ARG(argc,argv);
    MyApp app(4);

    MyMsgServer *server = new MyMsgServer();
    server->SetReuseSock();
    server->Bind();
    server->Listen();
    server->SetNonblock(true);
    app.AddEvent(server);

    return app.Exec();
}
