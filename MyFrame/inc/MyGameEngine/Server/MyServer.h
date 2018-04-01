#ifndef MyServer_H
#define MyServer_H

/**
 * TODO(lkp): something need to do...
 */


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
    static MyGameServer* sInstance;
    static bool StaticInit();

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
