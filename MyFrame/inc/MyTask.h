#ifndef __MYTASK_H__
#define __MYTASK_H__
#include "MyCommon.h"
#include "MyThread.h"
#include "MyEvent.h"

namespace my_master {

class MyTask : public my_master::MyEvent, public my_master::MyThread
{
    friend class MyApp;
public:
    enum TASK_STATUS{
        TASK_RUNING,
        TASK_IDLE,
        TASK_STOP,
        TASK_ERROR
    };
public:
    MyTask();
    virtual ~MyTask();
    virtual void Run() override;
    virtual void OnInit() override;
    virtual void OnExit() override;

    /**
     * Update() - 需要循环的函数
     * @evs: 需要处理的事件队列
     */
    virtual void Update(my_master::MyList* evs); // virtual method
    ////////////////////////////////////////////// override MyEvent method
    int GetEventFd(){return m_msgFd[1];}
    EVENT_TYPE GetEventType(){return EVENT_TYPE::EV_TASKFD;}
    uint32_t GetEpollEventType(){ return EPOLLIN; }
    void* CallBackFunc(MyEvent *){ return NULL; }


    void Quit();                                // task quit
    int SendMsg(const char *buf, int len);      // invoke by MyApp
    int RecvMsg(char *buf, int len);            // invoke by MyApp
    int WaitEvent();                            // invoke by myself
    int ProcessMsg();                           // process MyApp event(do not use)

    /**
     * 该线程是否是需要循环的线程
     * 在子类中初始化调用此函数，如果需要使用此功能
     */
    void SetLoop(bool b){ m_needLoop = b; }
    bool IsLoop(){ return m_needLoop; }

    /**
     * 该线程是否只处理指定自己ID的事件
     * 在子类中初始化调用此函数，如果需要使用此功能
     */
    void SetSpecifledEv(bool b){ m_specifledEv = b; }
    bool IsProcessSpecifledEv(){ return m_specifledEv; }

    /**
     * 设置线程数组, 便于索引, 索引使用Identify
     */
    static MyTask* l_tasks[100];
    static int l_cur__max_identify;
private:
    int TaskWork();
    int CreateSockPair();   // communication between self and mainthread
    void ClearResource();
private:
    my_master::MyList m_send;              // send queue
    my_master::MyList m_recv;              // recv queue
    my_master::MyList m_que;               // work queue, save MyEvent class

    my_master::MyList m_ev_que;            // 由工作队列中的事件产生的事件队列, 交由Update函数处理的队列

    //TASK_STATUS m_status;                // current thread runing status
    int m_msgFd[2];                        // 0 used by myself, 1 used by MyApp
    uint8_t m_msgBuf[MSG_LEN];
    bool m_isQuit;
    bool m_needLoop;                       // 该线程是否是需要循环的线程
    bool m_specifledEv;                    // 是否只处理指定自己ID的事件
};

} // end namespace
#endif // MYTASK_H
