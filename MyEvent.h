#ifndef MYEVENT_H
#define MYEVENT_H
#include "Common.h"
#include <sys/epoll.h>

namespace my_master {
//typedef void*(*callback_t)(void*);
class MyEvent : public MyNode
{
    friend class MyTask;
    friend class MyApp;
public:
    enum EVENT_TYPE{
        FILE,
        SOCK,
        TASK,
        NONE
    };
public:
    MyEvent();
    ~MyEvent();
    virtual int GetEventFd() = 0;
    virtual EVENT_TYPE GetEventType() = 0;
    virtual uint32_t GetEpollEventType() = 0;
    virtual void* CallBackFunc(MyEvent*) = 0;
    //void SetCallBack(callback_t cb);
};

} // end namespace
#endif // MYEVENT_H
