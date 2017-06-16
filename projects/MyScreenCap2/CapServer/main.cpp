#include <iostream>
#include "MyFrame.h"
#include "MyTcp.h"
#include "MyServer.h"

using namespace my_master;
using namespace std;

int main(int argc, char *argv[])
{
    MyApp app{4,1024};
    // create account table
    // define msg protocol
    // using tcp
    // match ip, account
    //
    MyTcpServer *server = new MyTcpServer("",4399);
    server->Bind();
    server->Listen();
    app.AddEvent(server);

    MyServer* widget = new MyServer;
    widget = widget;

    for(int i = 0; i < 10; ++i)
    {
        std::cout << widget->GetAccount() << std::endl;
        std::cout << widget->Getpass() << std::endl;
    }

    return app.Exec();
}
