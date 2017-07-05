#include "MyServer.h"
#include "MyTcp.h"
#include "MyUdp.h"

using namespace my_master;

MyServer::MyServer()
    :MyCtrlObj("server")
{
#if 1

#endif
}

bool MyServer::Event(MyEvent* ev)
{
    if(ev->GetClassType() == "MyTcpServer")
    {
        MyTcpServer* serv = (MyTcpServer*)ev;
        sockaddr_in addr;
        while(1)
        {
            int fd = serv->Accpet(&addr);
            if(fd < 0)
                break;
            // random account and pass
            std::string account = GetAccount();
            std::string pass = Getpass();
            MyCapConnect *recv = new MyCapConnect(account,pass,fd,addr);
            printf("get client connect fd : %d, port %u, ip %s\n",
                   fd,
                   MyNet::GetAddrPort(&addr),
                   MyNet::GetAddrIp(&addr).c_str());
            // send account and pass TODO...
            int len;
            char* buf = recv->BuildAccountAndPass(account,pass,&len);
            int res = recv->EasyWrite(buf,len);
            assert(res == len);
            MySelfProtocol::FreeBuf(buf);

            MyApp::theApp->AddEvent(recv);
        }
        MyApp::theApp->AddEvent(ev);
    }
    return true;
}

std::string MyServer::GetAccount()
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
    if(REQUEST(temp) != NULL)
        goto again;
    return temp;
}

std::string MyServer::Getpass()
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

















