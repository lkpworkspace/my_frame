#ifndef __MYTASK_H__
#define __MYTASK_H__
#include "MyCommon.h"
#include "MyThread.h"
#include "MyEvent.h"

namespace my_master {

class MyTask : public my_master::MyThread, public my_master::MyEvent
{
    friend class MyApp;
public:
    enum TASK_STATUS{
        RUNING,
        IDLE,
        STOP,
        ERROR
    };
public:
    MyTask();
    virtual ~MyTask();
    void Run();    // override
    void OnInit(); // override
    void OnExit(); // override
    ////////////////////////////////////////////// override MyEvent method
    int GetEventFd(){return m_msgFd[1];}
    EVENT_TYPE GetEventType(){return EVENT_TYPE::TASKFD;}
    uint32_t GetEpollEventType(){ return EPOLLIN; }
    void* CallBackFunc(MyEvent *){return NULL;}
    std::string GetClassType(){return "MyTask";}

    void Quit();                                // task quit
    int SendMsg(const char *buf, int len);      // invoke by MyApp
    int RecvMsg(char *buf, int len);            // invoke by MyApp
    int WaitEvent();                            // invoke by myself
    int ProcessMsg();                           // process MyApp event(do not use)
private:
    int TaskWork();
    int CreateSockPair();   // communication between thread
    void ClearResource();
private:
    my_master::MyList m_recv;            // recv queue
    my_master::MyList m_que;             // work queue, save MyEvent class

    //TASK_STATUS m_status;                // current thread runing status
    int m_msgFd[2];                      // 0 used by myself, 1 used by MyApp
    uint8_t m_msgBuf[MSG_LEN];
    bool m_isQuit;
};

} // end namespace
#endif // MYTASK_H