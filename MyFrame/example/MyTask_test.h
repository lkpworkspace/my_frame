#ifndef MYTASK_TEST_H
#define MYTASK_TEST_H
#include "MyFrame.h"
#include "MyTcp.h"

/**
 * @brief The MyEventTest class
 */
class MyEventTest : public MyEvent
{
public:
    MyEventTest()
    {
        SetSendIdentify(m_send_task);
    }

    void Print()
    {
        printf("test count %d: %s\n",m_test_cout++, m_send_str.c_str());
    }
    /// override
    EVENT_TYPE GetEventType(){return EV_NONE;}
    void* CallBackFunc(MyEvent*){ return NULL; }


    std::string m_send_str;
    static int m_test_cout;
    static int m_send_task;
};
int MyEventTest::m_send_task = -1;
int MyEventTest::m_test_cout = -1;


/**
 * @brief The MyTestSock class
 */
class MyTestSock : public MyEasyTcpSocket
{
public:
    MyTestSock(int fd, sockaddr_in addr)
        :MyEasyTcpSocket(fd, addr)
    {

    }

    virtual int Frame(const char* buf, int len)
    {
        MyEventTest* met = new MyEventTest();
        printf("something get: %s\n",buf);
        if(len == 0)
        {
            met->m_send_str = "";
            return false;
        }
        met->m_send_str = buf;
        AddSendEv(met);
        return true;
    }
};
/**
 * @brief The MyTestServer class
 */
class MyTestServer : public MyTcpServer
{
public:
    MyTestServer()
        :MyTcpServer("127.0.0.1",4399)
    {

    }

    virtual ~MyTestServer()
    {

    }

    virtual void* CallBackFunc(MyEvent *ev)
    {
        MyTestServer* serv = (MyTestServer*)ev;
        sockaddr_in addr;
        while(1)
        {
            int fd = serv->Accpet(&addr);
            if(fd < 0)
                break;
            MyTestSock *recv = new MyTestSock(fd,addr);
            MyDebugPrint("get client connect fd : %d, port %u, ip %s\n",
                   fd,
                   MyNet::GetAddrPort(&addr),
                   MyNet::GetAddrIp(&addr).c_str());

            MyApp::theApp->AddEvent(recv);
        }
        MyApp::theApp->AddEvent(ev);
    }
};









class MyTaskTest : public MyTask
{
public:
    MyTaskTest()
    {
        SetLoop(true);
        SetSpecifledEv(true);
    }

    virtual ~MyTaskTest()
    {

    }

    virtual void Update(MyList *evs)
    {// delete MyEvent when used
        MyEvent* begin = (MyEvent*)evs->Begin();
        MyEvent* end = (MyEvent*)evs->End();
        MyEvent* me = NULL;
        MyEventTest* temp = NULL;
        for(;begin != end;)
        {
            me = (MyEvent*)begin->next;
            evs->Del(begin,false);
            // TODO...
            temp = (MyEventTest*)begin;
            temp->Print();
            // TODO end
            delete begin;
            begin = me;
        }
//        printf("loop\n");
        usleep(1000 * 500);
    }

    static void Test()
    {
        MyApp app(4);

        // provess task
        MyTaskTest* tt = new MyTaskTest();
        MyEventTest::m_send_task = tt->GetIdentify();
        MyApp::theApp->AddEvent(tt);

        // tcp server
        MyTestServer* server = new MyTestServer();
        server->SetReuseSock();
        server->Bind();
        server->Listen();
        app.AddEvent(server);

        app.Exec();
    }
private:
};



#endif // MYTASK_TEST_H
