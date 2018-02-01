#ifndef __MyAllEvent_test_h__
#define __MyAllEvent_test_h__
#include "MyFrame.h"
#include "MyTest.h"
#include "MyMouseEvent.h"

class MyMouse : public MyMouseEvent
{ // maybe do not us the method
public:
    void* CallBackFunc(MyEvent *ev)
    {
        MyMouseEvent::CallBackFunc(ev);
        printf("button type = %d, x = %d, y = %d\n",
               GetMouseType(),GetRelX(),GetRelY());
        MyApp::theApp->AddEvent(ev);
        return NULL;
    }
};

#if 0
class AllEv : public MyAllEvent
{
public:
    bool Event(MyEvent *ev)
    {
        switch (ev->GetClassType()) {
        case MyEvent::CLASS_TYPE::TCPSERVER:
        {
            MyTcpServer* serv = (MyTcpServer*)ev;
            sockaddr_in addr;
            while(1)
            {
                int fd = serv->Accpet(&addr);
                if(fd < 0)
                    break;
                MyTcpSocket *recv = new MyTcpSocket(fd,addr);
                printf("get client connect fd : %d, port %u, ip %s\n",fd,
                       MyNet::GetAddrPort(&addr),
                       MyNet::GetAddrIp(&addr).c_str());
                MyApp::theApp->AddEvent(recv);
            }
            MyApp::theApp->AddEvent(ev);
        }
            break;
        case MyEvent::CLASS_TYPE::TCPSOCKET:
        {
            char buf[1024] = {0};
            int res;
            /* res:
             * =0 client quit
             * >0 data is coming
             * <0 end of file
            */
            MyTcpSocket* sock = (MyTcpSocket*)ev;
            while(1)
            {
                res = sock->Read(buf,1024);
                if(res <= 0)
                    break;
                printf("%s:%d: read %d:%s\n",sock->GetIp().c_str(),sock->GetPort(),res,buf);
            }
            if(res != 0)
            {
                MyApp::theApp->AddEvent(ev);
            }
            else
            {
                delete sock;
                printf("client quit\n");
            }
        }
            break;
        case MyEvent::CLASS_TYPE::MOUSE:
        {
            MyMouseEvent* e = (MyMouseEvent*)ev;
            printf("button type = %d, x = %d, y = %d\n",
                   e->GetMouseType(),e->GetRelX(),e->GetRelY());
            MyApp::theApp->AddEvent(ev);
        }
            break;
        case MyEvent::CLASS_TYPE::UDPCLASS:
        {
            static int count = 0;
            MyUdp* e = (MyUdp*)ev;
            char* buf;
            int len = 0;

            MyAddrInfo addr = e->RecvData(&buf,len);
//            printf("ip : %s, port : %u, recv: %s\n",
//                   addr.GetIp().c_str(),
//                   addr.GetPort(),
//                   buf);
            MyDebugPrint("count %d\r",count++);
            MyApp::theApp->AddEvent(ev);
        }
            break;
        case MyEvent::CLASS_TYPE::FILECLASS:
        {
            MyFileEvent::GetInfo();
        }
            break;
        default:
            break;
        }
        return true;
    }
};
#endif



class MyAllEvent_test
{
public:
    MyAllEvent_test()
    {

    }

    void test()
    {

    }
};




#endif
