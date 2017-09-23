#include "MyMsgServer.h"
#include "MyMsgConnect.h"
#include "MyMsgManager.h"

using namespace my_master;


MyMsgServer::MyMsgServer()
    :MyCtrlObj("MyMsgServer"),
      MyTcpServer(IP,PORT)
{
    m_manager = new MyMsgManager();
}

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
        MyMsgConnect *recv = new MyMsgConnect(fd,addr);
        MyDebugPrint("get client connect fd : %d, port %u, ip %s\n",
               fd,
               MyNet::GetAddrPort(&addr),
               MyNet::GetAddrIp(&addr).c_str());

        //recv->WelcomeMsg();
        //recv->InitAccountInfo("123456789");
        MyApp::theApp->AddEvent(recv);
    }
    MyApp::theApp->AddEvent(ev);

    return NULL;
}
