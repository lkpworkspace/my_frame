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
    switch (ev->GetClassType())
    {
    case MyEvent::CLASS_TYPE::TCPSERVER:
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
            printf("get client connect fd : %d, port %u, ip %s\n",fd,
                   MyNet::GetAddrPort(&addr),
                   MyNet::GetAddrIp(&addr).c_str());
            // send account and pass
            int len;
            char* buf = BuildAccountAndPass(account,pass,&len);
            int res = recv->Write(buf,len);
            assert(res != len);
            MySelfProtocol::FreeBuf(buf);

            MyApp::theApp->AddEvent(recv);
        }
        MyApp::theApp->AddEvent(ev);
    }
        break;
    case MyEvent::CLASS_TYPE::TCPSOCKET:
    {
        uint16_t head;
        int res;
        /* res:
         * =0 client quit
         * >0 data is coming
         * <0 end of file
        */
        MyCapConnect* sock = (MyCapConnect*)ev;
        if(sock->IsComplete())
        {/* read head */
            res = sock->Read((char*)&head,sizeof(head));
            if(res == sizeof(head))
            {
                // print info
                printf("account: %s pass: %s\n",
                       sock->GetAccount().c_str(),
                       sock->GetPass().c_str());
                printf("%s:%d: read %d\n",
                       sock->GetIp().c_str(),
                       sock->GetPort(),
                       res);
                // end print
                Handle(sock,head);
            }
        }else
        {/* complete one frame */
            Handle(sock,sock->GetHead());
            printf("complete one frame\n");
        }

        if(res == 0)
        {
            if(sock)
            {
                delete sock;
                sock = nullptr;
            }
            printf("client quit\n");
        }

        if(res != 0)
        {
            MyApp::theApp->AddEvent(ev);
        }
    }
        break;
    default:
        break;
    }
    return true;
}

void MyServer::Handle(MyCapConnect* sock, uint16_t head)
{
    switch(head)
    {
    case 0x0003:
        HandleConnectMsg(sock);
        break;
    case 0x0004:
        HandleData(sock);
        break;
    case 0xffff:
        MyDebugPrint("error in this case");
        break;
    default:
        break;
    }
}

void MyServer::HandleConnectMsg(MyCapConnect* sock)
{
    char ch;
    int res;

    if(sock->IsComplete())
    {
        sock->m_conn_account.clear();
        sock->m_conn_pass.clear();
        sock->m_index = 0;
    }
    while(1)
    {
        res = sock->Read(&ch,1);
        if(res == 1)
        {
            if(ch == '\0')
            {
                if(sock->m_index == 1)
                    sock->SetHeadAndIndex(0xffff,0);
                else
                    sock->m_index++;
                break;
            }
            if(sock->m_index == 1)
                sock->m_conn_pass += ch;
            else
                sock->m_conn_account += ch;
        }else if(res == 0)
        {
            // client quit
            if(sock){
                delete sock;
                sock = nullptr;
            }
            printf("client quit\n");
        }else
        {
            // res < 0
            break;
        }
    }

    printf("%s %s\n",
           sock->m_conn_account.c_str(),
           sock->m_pass.c_str());
    if(sock->IsComplete())
    {
        if(REQUEST(sock->m_conn_account) != NULL)
        {
            int len;
            char* buf = BuildMsg(0x01,&len);
            int res = sock->Write(buf,len);
            assert(res == len);
            MySelfProtocol::FreeBuf(buf);
        }else
        {
            int len;
            char* buf = BuildMsg(0x02,&len);
            int res = sock->Write(buf,len);
            assert(res == len);
            MySelfProtocol::FreeBuf(buf);
        }
    }
}

void MyServer::HandleConnectMsg(char* buf, int len)
{

}

void MyServer::HandleData(MyCapConnect*sock)
{

    uint16_t len;

    if(sock->IsComplete())
    {
        sock->data_len = 0;
        sock->m_index = 0;
    }
    int res = sock->Read((char*)&len,sizeof(len));
    assert(res == sizeof(len));
    while(1)
    {

    }
}

void MyServer::HandleData(char* buf, int len)
{
    int data_len,index = 2;
    std::string account = MySelfProtocol::HandleString(index,buf,len);
    index += (account.size() + 1);

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

char* MyServer::BuildMsg(uint8_t msg_num, int *outlen)
{
    uint16_t head = 0x0001;
    int len, index = 0;
    char* buf = MySelfProtocol::GetBuf(&len);
    index += MySelfProtocol::BuildHeader(head,buf,len);
    index += MySelfProtocol::BuildChar(msg_num,index,buf,len);
    *outlen = index;
    return buf;
}

char* MyServer::BuildAccountAndPass(std::string account,
                                    std::string pass,
                                    int* outlen)
{
    uint16_t head = 0x0002;
    int len, index = 0;
    char* buf = MySelfProtocol::GetBuf(&len);
    index += MySelfProtocol::BuildHeader(head,buf,len);
    index += MySelfProtocol::BuildString(account.c_str(),index,buf,len);
    index += MySelfProtocol::BuildString(pass.c_str(),index,buf,len);
    *outlen = index;
    return buf;
}















