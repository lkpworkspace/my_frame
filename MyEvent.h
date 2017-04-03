#ifndef MYEVENT_H
#define MYEVENT_H
#include "Common.h"
#include <sys/epoll.h>
#include "MyList.h"
namespace my_master {

class MyEvent : public my_master::MyNode
{
    friend class MyTask;
public:
    enum EVENT_TYPE{
        FILE,
        SOCK,
        TASK,
        NONE
    };
    enum CLASS_TYPE{
        TCPSERVER,
        TCPCLIENT,
        TCPSOCKET,
        UDP,
        UDPSOCKET,
        MOUSE,
        BASECLASS,
        TASKCLASS
    };

public:
    MyEvent();
    ~MyEvent();
    virtual int GetEventFd() = 0;
    virtual EVENT_TYPE GetEventType() = 0;
    virtual CLASS_TYPE GetClassType() = 0;
    virtual uint32_t GetEpollEventType() = 0;
protected:
    virtual void* CallBackFunc(MyEvent*) = 0;
};

} // end namespace
#endif // MYEVENT_H
