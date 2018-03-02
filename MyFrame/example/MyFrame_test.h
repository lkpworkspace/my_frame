#ifndef MYFRAME_TEST_H
#define MYFRAME_TEST_H

#include "MyFrame.h"

/**
 * 示例程序简述：
 *      创建一个TCP服务器对象(MyTcpServerTest)
 *      服务器接收到客户端连接后创建客户端连接对象(MyTcpSockTest)
 *      客户端对象收到客户端发送的消息后，
 *      将消息打包成(MyMsgTest)类发送到指定线程处理队列上(MyTaskTest)
 */


#if 1
#define IP "127.0.0.1"
#else
#define IP "172.16.212.139"
#endif
#define PORT 4399


/**
 * 普通消息对象，用于进行线程间传递事件消息使用
 */
class MyMsgTest : public myframe::MyEvent
{
public:
    /**
     * 由 MyMsgPool 调用此函数创建该类
     */
    static MyEvent* Create()
    {
        return (MyEvent*)(new MyMsgTest());
    }

    MyMsgTest()
    {
        /**
         * 设置消息的名称
         */
        SetObjName("MYMSG_test");
        /**
         * 设置该消息由那个线程处理
         */
        SetSendIdentify(m_send_task);
    }

    void Print()
    {
        printf("[print   ]: test count %d: %s\n",m_test_cout, m_send_str.c_str());
    }

    //////////////////////////////////////// override
    EVENT_TYPE GetEventType(){return EV_NONE;}
    void* CallBackFunc(MyEvent*){
        MyDebug("[callback]: test count %d: %s",m_test_cout, m_send_str.c_str());
        MyDebugPrint("[callback]: test count %d: %s\n",m_test_cout, m_send_str.c_str());
        ++m_test_cout;
        return NULL;
    }


    std::string m_send_str;
    static int m_test_cout;
    static int m_send_task;
};
int MyMsgTest::m_send_task = -1;
int MyMsgTest::m_test_cout = -1;


/**
 * TCP服务器收到客户端连接就会创建该对象，
 * 表示一个客户端连接，
 * 并将该对象加入到监听队列中，
 * 有事件会回调Frame函数进行处理
 */
class MyTcpSockTest : public MyEasyTcpSocket
{
public:
    MyTcpSockTest(int fd, sockaddr_in addr)
        :MyEasyTcpSocket(fd, addr)
    {
    }

    /**
     * Frame()
     * @buf:   传入的消息的buf
     * @len:   消息的长度(如果为0,表示客户端退出)
     * Return: 返回true表示继续监听该事件，false表示不再监听此事件
     */
    virtual int Frame(const char* buf, int len)
    {
//        MyMsgTest* met = new MyMsgTest();
        MyMsgTest* met = (MyMsgTest*)MyMsgPool::Instance()->Get("MYMSG_test");
        MyDebug("Get Msg: %s\n",buf);
        MyDebugPrint("Get Msg: %s, %p\n",buf,met);
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
 * 创建一个TCP服务器对象，
 * 任务是接受客户端请求的连接，
 * 并将连接对象 加入到监听队列中
 */
class MyTcpServerTest : public MyTcpServer
{
public:
    MyTcpServerTest()
        :MyTcpServer(IP,PORT)
    {

    }

    virtual ~MyTcpServerTest()
    {

    }

    virtual void* CallBackFunc(MyEvent *ev)
    {
        MyTcpServerTest* serv = (MyTcpServerTest*)ev;
        sockaddr_in addr;
        while(1)
        {
            int fd = serv->Accpet(&addr);
            if(fd < 0)
                break;
            MyTcpSockTest *recv = new MyTcpSockTest(fd,addr);
            MyDebugPrint("get client connect fd : %d, port %u, ip %s\n",
                   fd,
                   MyNet::GetAddrPort(&addr),
                   MyNet::GetAddrIp(&addr).c_str());
            MyDebug("get client connect fd : %d, port %u, ip %s\n",
                    fd,
                    MyNet::GetAddrPort(&addr),
                    MyNet::GetAddrIp(&addr).c_str());

            MyApp::theApp->AddEvent(recv);
        }
        MyApp::theApp->AddEvent(ev);
        return NULL;
    }
};


class MyTaskTest : public MyTask
{
public:
    MyTaskTest()
    {
        /**
         * 设置线程是帧循环的，而不是基于事件驱动的
         */
        //SetLoop(true);

        /**
         * 设置该线程只处理指定自己ID的事件
         */
        SetSpecifledEv(true);
    }

    virtual ~MyTaskTest()
    {
    }

    ///////////////////////////////////////////////// 可以重写的函数
    /**
     * 线程初始化函数，仅初始化调用一次，
     * note that: 需要调用父类中的方法
     */
    //virtual void OnInit();

    /**
     * 线程退出函数，仅退出调用一次，
     * note that: 需要调用父类中的方法
     */
    //virtual void OnExit();

    /**
     * 每次线程收到事件便会调用此函数，如果设置线程为无限循环的(SetLoop)，
     * 则不管有没有事件该线程会一直调用此函数，
     * 所以需要在Update中加一点延时
     */
    virtual void Update(MyList *evs)
    {
        MyEvent* begin = (MyEvent*)evs->Begin();
        MyEvent* end = (MyEvent*)evs->End();
        MyEvent* next = NULL;
        MyMsgTest* temp = NULL;

        for(;begin != end;)
        {
            next = (MyEvent*)begin->next;
            evs->Del(begin,false);

            //TODO(lkp): 事件处理
            temp = (MyMsgTest*)begin;
            temp->Print();
            // TODO end

            MyMsgPool::Instance()->Free(temp);
            begin = next;
        }


        // TODO...
#if 1
        static int counter = 0;
        MyDebug("loop %d\n", counter);
        MyDebugPrint("loop %d\n", counter);
        ++counter;
#endif
        //usleep(1000 * 1000 * 60 * 60);

        // TODO end
    }


    /**
     * 入口函数
     */
    static void Test()
    {
        //MyHelp::DaemonInit();
        MyApp app(4);

        // msgpool register
        MyMsgPool::Instance()->RegMsg("MYMSG_test", MyMsgTest::Create);

        // process task
        MyTaskTest* tt = new MyTaskTest();
        MyMsgTest::m_send_task = tt->GetIdentify();
        MyApp::theApp->AddEvent(tt);

        // tcp server
        MyTcpServerTest* server = new MyTcpServerTest();
        server->SetReuseSock();
        server->Bind();
        server->Listen();
        server->SetNonblock(true);
        app.AddEvent(server);

        app.Exec();
    }

private:
};



#endif // MYTASK_TEST_H
