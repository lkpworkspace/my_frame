#include "MyFrame.h"

#include "MyMouseEvent.h"
#include "MyTcp.h"

using namespace my_master;
//#define TEST

#ifdef TEST
class A : public MyNode
{
public:
    A(int a){
        this->a = a;
    }
    int a;
};

int main()
{
    int x = 10;
    MyDebug("okok%d\n",x);
#if 1
    MyList list;
    list.AddTail(new A(10));

    A* begin = (A*)list.Begin();
    A* end = (A*)list.End();
    printf("begin pointer %p\n",begin);
    printf("end pointer %p\n",end);
    while(begin != end)
    {
        std::cout << ((A*)begin)->a << std::endl;
        printf("begin pointer %p\n",begin);
        begin = (A*)(begin->next);
        printf("begin pointer %p\n",begin);
    }
    std::getchar();
#endif
    return 0;
}
#else
#if 1
// server
class MyRecv : public MyTcpSocket
{ // maybe do not us the method
public:
    MyRecv(int fd, sockaddr_in addr)
        :MyTcpSocket(fd,addr)
    {
        Common::SetNonblock(fd,true);
    }

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
        SetNonblock(true);
    }

    void* CallBackFunc(MyEvent *ev)
    {
        MyTcpServer* serv = (MyTcpServer*)ev;
        sockaddr_in addr;
        socklen_t len;
        while(1)
        {
            int fd = serv->Accpet(&addr,&len);
            if(fd < 0)
                break;
            MyRecv *recv = new MyRecv(fd,addr);
            printf("get client fd : %d\n",fd);
            MyApp::theApp->AddEvent(recv);
        }
        MyApp::theApp->AddEvent(ev);
    }
};

class MyMouse : public MyMouseEvent
{ // maybe do not us the method
public:
    void* CallBackFunc(MyEvent *ev)
    {
        MyMouseEvent::CallBackFunc(ev);
        printf("button type = %d, x = %d, y = %d\n",
               GetMouseType(),GetRelX(),GetRelY());
        MyApp::theApp->AddEvent(ev);
    }
};

class AllEv : public MyAllEvent
{
public:
    bool Event(MyEvent *ev)
    {
        if(ev->GetClassType() == MyEvent::CLASS_TYPE::TCPSERVER)
        {
            MyTcpServer* serv = (MyTcpServer*)ev;
            sockaddr_in addr;
            socklen_t len;
            while(1)
            {
                int fd = serv->Accpet(&addr,&len);
                if(fd < 0)
                    break;
                MyTcpSocket *recv = new MyTcpSocket(fd,addr);
                printf("get client fd : %d\n",fd);
                MyApp::theApp->AddEvent(recv);
            }
            MyApp::theApp->AddEvent(ev);
        }
        if(ev->GetClassType() == MyEvent::CLASS_TYPE::TCPSOCKET)
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
                if(res < 0)
                    break;
                printf("read %d:%s\n",res,buf);
            }
            if(res != 0)
                MyApp::theApp->AddEvent(ev);
            else
                printf("client quit\n");
        }
        if(ev->GetClassType() == MyEvent::CLASS_TYPE::MOUSE)
        {
            MyMouseEvent* e = (MyMouseEvent*)ev;
            printf("button type = %d, x = %d, y = %d\n",
                   e->GetMouseType(),e->GetRelX(),e->GetRelY());
            MyApp::theApp->AddEvent(ev);
        }
        return true;
    }
};

int main()
{
    MyApp app{4,1024};

    // tcp test
    MyTcpServer *server = new MyTcpServer("",9999);
    server->Bind();
    server->Listen(10);
    server->SetNonblock(true);
    app.AddEvent(server);

    // mouse test
    MyMouseEvent* mouse = new MyMouseEvent;
    app.AddEvent(mouse);

    // ev procees
    AllEv* widget = new AllEv;

    return app.Exec();
}
#else
// client
int main(int argc, char *argv[])
{
    MyTcp client("127.0.0.1",19999,false);
    client.Connect();

    char buf[20] = {0};
    int res = 0;
    while(true)
    {
        fscanf(stdin,"%s",buf);
        res = client.Write(buf,20);
        //printf("read %d byte : %s\n",res,buf);
        memset(buf,0,20);
    }
    return 0;
}
#endif
#endif
