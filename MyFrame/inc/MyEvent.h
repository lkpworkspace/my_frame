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
        FILEFD,
        SOCKFD,
        TASKFD,
        NONE
    };
#if 0
    enum CLASS_TYPE{
        BASECLASS,
        TASKCLASS,
        TCPSERVER,
        TCPCLIENT,
        TCPSOCKET,
        UDPCLASS,
        UDPSOCKET,
        MOUSE,
        KEY,
        FILECLASS
    };
#endif
public:
    MyEvent();
    virtual ~MyEvent();
    virtual int GetEventFd() = 0;
    virtual EVENT_TYPE GetEventType() = 0;     // maybe not most need
    //virtual CLASS_TYPE GetClassType() = 0;
    virtual std::string GetClassType() = 0;
    virtual uint32_t GetEpollEventType() = 0;
protected:
    virtual void* CallBackFunc(MyEvent*) = 0;
};

} // end namespace
#endif // MYEVENT_H
