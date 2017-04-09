#ifndef MYMOUSEEVENT_H
#define MYMOUSEEVENT_H
#include <sys/epoll.h>
#include "MyEvent.h"
namespace my_master {
class MyMouseEvent : public my_master::MyEvent
{
public:
    MyMouseEvent();
    ~MyMouseEvent();
    ///////////////////////////////////////
    /// override myevent method
    int GetEventFd(){return m_fd;}
    EVENT_TYPE GetEventType(){return EVENT_TYPE::FILEFD;}
    uint32_t GetEpollEventType(){return EPOLLIN;}
    CLASS_TYPE GetClassType(){return CLASS_TYPE::MOUSE;}

    /*
     *  0: left button up
     *  1: left button down
     *  2: right button down
     */
    int GetMouseType();
    int GetRelX();
    int GetRelY();
protected:
    ///////////////////////////////////////
    /// override myevent method
    virtual void* CallBackFunc(MyEvent*);
private:
    int m_fd;
    char m_buf[3];
};
} // end namespace
#endif // MYMOUSEEVENT_H
