#ifndef __MyTcp_test_h__
#define __MyTcp_test_h__
#include "MyFrame.h"
#include "MyTest.h"
#include "MyTcp.h"

class MyRecv : public MyTcpSocket
{ // maybe do not us the method
public:
    MyRecv(int fd, sockaddr_in addr)
        :MyTcpSocket(fd,addr)
    {}

    void* CallBackFunc(MyEvent *ev)
    {
        char buf[1024] = {0};
        /* res:
         * =0 client quit
         * >0 data is coming
         * <0 end of file
        */
        int res;
        MyTcpSocket* sock = (MyTcpSocket*)ev;
        do
        {
            res = sock->Read(buf,1024);
            printf("read %d:%s\n",res,buf);
        }while(res > 0);
        if(res != 0)
            MyApp::theApp->AddEvent(ev);
        else
            printf("client quit\n");
        return NULL;
    }
};

class MyServer: public MyTcpServer
{ // maybe do not us the method
public:
    MyServer(std::string ip,uint16_t port)
        :MyTcpServer(ip,port)
    {
        Bind();
        Listen();
    }

    void* CallBackFunc(MyEvent *ev)
    {
        MyTcpServer* serv = (MyTcpServer*)ev;
        sockaddr_in addr;
        while(1)
        {
            int fd = serv->Accpet(&addr);
            if(fd < 0)
                break;
            MyRecv *recv = new MyRecv(fd,addr);
            printf("get client fd : %d\n",fd);
            MyApp::theApp->AddEvent(recv);
        }
        MyApp::theApp->AddEvent(ev);
        return NULL;
    }
};







class MyTcp_test
{
public:
    MyTcp_test()
    {

    }

    void test()
    {

    }

    void test2()
    {

    }
};




#endif
