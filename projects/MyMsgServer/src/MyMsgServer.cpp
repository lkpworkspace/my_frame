#include "../inc/MyMsgServer.h"
#include "MyApp.h"
#include "../inc/MyConnect.h"

using namespace my_master;


MyMsgServer::MyMsgServer()
    :MyCtrlObj("MyMsgServer"),
      MyTcpServer(IP,PORT)
{}

MyMsgServer::~MyMsgServer()
{}

void* MyMsgServer::CallBackFunc(MyEvent *ev)
{
    MyMsgServer* serv = (MyMsgServer*)ev;
    sockaddr_in addr;
    while(1)
    {
        int fd = serv->Accpet(&addr);
        if(fd < 0)
            break;
        MyConnect *recv = new MyConnect(fd,addr);
        printf("get client connect fd : %d, port %u, ip %s\n",
               fd,
               MyNet::GetAddrPort(&addr),
               MyNet::GetAddrIp(&addr).c_str());

        recv->WelcomeMsg();
        MyApp::theApp->AddEvent(recv);
    }
    MyApp::theApp->AddEvent(ev);

    return NULL;
}
