#ifndef MyServer_H
#define MyServer_H

/**
 * TODO(lkp): something need to do...
 */



#if 0
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
#endif

/**
 * 创建一个TCP的游戏服务器对象，
 * 任务是接受客户端请求的连接，
 * 并将连接对象 加入到监听队列中
 */
class MyGameTcpServer : public MyTcpServer
{
public:
    MyGameTcpServer();

    virtual ~MyGameTcpServer();

    virtual void* CallBackFunc(MyEvent *ev);
};


class MyGameEngineTask : public MyTask
{
public:
    MyGameEngineTask();

    virtual ~MyGameEngineTask();

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
    virtual void Update(MyList *evs);
private:
};

/**
 * MyServer:
 *  Run()
 *  DoFrame()
 */
class MyGameServer : MyEngine
{
    friend class MyGameEngineTask;
public:

    static bool StaticInit();
    static MyGameServer* sInstance;

    int Go();

    /**
     * 入口函数
     */
    virtual int Run();

    virtual void DoFrame();

    /**
     * create player
     */
    void SpawnPlaneForPlayer(int inPlayerId);

    int mGameEngineTaskId;

    MyMsgPool mMsgPool;
private:
    MyGameServer();

    void SetupWorld();

    MyList mGmameMsgs;
};


#endif // MYTASK_TEST_H
