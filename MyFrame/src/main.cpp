#include "../inc/MyFrame.h"

#include "../inc/MyMouseEvent.h"
#include "../inc/MyTcp.h"
#include "../inc/MySqlite3.h"
#include "../inc/MyUdp.h"
#include "../inc/MyTFTP.h"
#include "../inc/MyNormalEvent.h"
#include "../inc/MyTimer.h"
#include "../inc/MyFileEvent.h"
#include <thread>
using namespace my_master;
#define TEST

#ifndef TEST

// server
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
void* QuitFunc(void*)
{
    printf("ok this app was exit\n");
    return nullptr;
}

void* TimerFunc(void*)
{
    printf("timeer\n");
    return nullptr;
}

int main(int argc, char** argv)
{
    UNUSE_ARG(argc,argv);
    MyApp app{1};
    app.SetQuitFunc(QuitFunc);

#if 0
    // file monitor test
    MyFileEvent* fileEvent = new MyFileEvent();
    MyFileEvent::StartMonitor("filetest.txt");
#endif

#if 1
    // timer test
    std::thread thr([&](){
        sleep(1);
        MyTimer* timer = new MyTimer(500);
        timer->SetCallFunc(&TimerFunc,NULL);
        timer->Start();
        sleep(5);
        timer->Stop();
        sleep(1);
        printf("second\n");
        timer->Start();
    });
    thr.detach();
#endif // end timer test

    // tcp test
//    MyTcpServer *server = new MyTcpServer("",9999);
//    server->Bind();
//    server->Listen(10);
//    app.AddEvent(server);

//    MyTcpServer *server1 = new MyTcpServer("",8888);
//    server1->Bind();
//    server1->Listen(10);
//    app.AddEvent(server1);

#if 0 // udp test
#if 0
    // udp server
    MyUdp *server = new MyUdp("",4399,true);
    server->Bind();
    app.AddEvent(server);
#else
    // udp client
    MyUdp *client = new MyUdp("",5555,false);
    app.AddEvent(client);
    std::thread thr([&](){
        sleep(1);
        MyAddrInfo info;
        info.SetPort(4399);
        info.SetIP("127.0.0.1");
        char buf[500];
        memset(buf,1,sizeof(buf));
        while(true)
        {
            usleep(1000*1);
            client->Write(info,buf,500);
        }
    });
    thr.detach();
#endif
#endif // end udp test

    // mouse test
    //MyMouseEvent* mouse = new MyMouseEvent;
    //app.AddEvent(mouse);

#if 0 // MyTFTP test
#if 0 // MyTFTP client
    MyTFTP* tftp = new MyTFTP("",5555,true);
    tftp->SetRootDir("./");
    app.AddEvent(tftp);

    std::thread thr([&](){
        sleep(1);
        MyAddrInfo info;
        info.SetPort(4399);
        info.SetIP("127.0.0.1");
        printf("argc %d\n",argc);
        if(argc >= 3)
        {
            if(!strcmp(argv[1],"get"))
            {
                // get file
                tftp->GetFile(info,argv[2]);
            }
            if(!strcmp(argv[1],"post"))
            {
                // upload file
                tftp->SendFile(info,argv[2]);
            }
            MyApp::theApp->Quit();
        }
    });
    thr.detach();
#else // MyTFTP server
    MyTFTP* tftp = new MyTFTP("",4399,true);
    tftp->SetRootDir("./");
    tftp->Bind();
    app.AddEvent(tftp);
#endif
#endif // end MyTFTP test

#if 0  // test MyNormalEvent
    // see MyTimer used MyNormalEvent
#endif

#if 0  // test MyApp quit
    std::thread thr([&](){
        char ch;
        sleep(1);
        while((ch = getchar()) == 'q')
        {
            MyApp::theApp->Quit();
            break;
        }
    });
    thr.detach();
#endif // end MyApp quit test

    // ev procees
#if 0
    AllEv* widget = new AllEv;
    widget = widget;
#endif
    return app.Exec();
}

#endif // end not define TEST












#ifdef TEST
int main()
{
    MyApp app(4);

    std::thread thr([&](){
        while(MyHelp::getch() != 'q')
        {
            app.DelLater(NULL,100);
        }
    });
    thr.detach();

    return app.Exec();
}


#include "../inc/MyWriteable.h"
int main4()
{
    int a = 0;
    void *p = &a;

    MyWriteable w;
    w.AddWriteEvent((my_master::MyEvent*)p);

    std::thread thr([&](){
        while(true)
        {
            sleep(1);
            sem_t* temp = w.SemFind((my_master::MyEvent*)p);
            sem_post(temp);
        }
    });
    thr.detach();

    while(true)
    {
        w.WaitWriteable((my_master::MyEvent*)p);
        printf("aaaa\n");
    }
    return 0;
}

class A : public MyNode
{
public:
    A(int a){
        this->a = a;
    }
    int a;
};
class B
{
public:
    B(int v){value = v;}
    static bool Cmp(const void* max, const void* min)
    {
        B* ma = (B*)max;
        B* mi = (B*)min;
        if(ma->value >= mi->value)
            return true;
        return false;
    }
public:
    int value;
};

int main3()
{
#if 0
    MyHeap<B*> heap(1024,&B::Cmp,true);
    B* b1 = new B(2);
    B* b2 = new B(5);
    B* b3 = new B(4);
    B* b4 = new B(9);
    B* b5 = new B(3);
    heap.Add(b1);
    heap.Add(b2);
    heap.Add(b3);
//    heap.Add(b4);
//    heap.Add(b5);
    for(int i = 0; i < heap.Count();++i)
    {
        printf("%d\n",heap.GetData(i)->value);
    }
#endif
    MyHeap<int> heap(10);
    heap.Add(10);
    heap.Add(20);
    heap.Add(56);
    heap.Add(30);
    for(int i = 0; i < heap.Count(); ++i)
    {
        std::cout << heap.GetData(i) << std::endl;
    }
    std::cout << std::endl;
    while(!heap.IsEmpty())
        std::cout << heap.Pop() << std::endl;
    return 0;
}

int main1()
{
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

    return 0;
}

int main2()
{
#if 1
    MySqlite3 db("mydb.db");
    db.Open();
    //db.ExecSql("create table test(id, name, age)");
#if 0
    for(int i = 0; i < 10; ++i)
        db.ExecSql("insert into test values(10,'haha',20)");
#else
    db.ExecSql("select * from test");
#if 0
    while(true)
    {
        std::vector<std::string> *row = db.GetRow();
        if(row)
        {
            for(int i = 0; i < row->size(); ++i)
            {
                std::cout <<(*row)[i] << " ";
                //(*db.GetColumnName())[i];
            }
            std::cout << std::endl;
        }
        else
        {
            std::cout << "no data"  << std::endl;
            break;
        }
    }
    for(int i = 0; i < db.GetColCount(); ++i)
    {
        std::cout << (*db.GetColumnName())[i] << std::endl;
    }
#else
    std::cout << db.GetValue(0,2);
#endif
#endif
    db.Close();
#endif
    return 0;
}

#endif
