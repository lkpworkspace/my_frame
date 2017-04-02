#ifndef MYMOUSEEVENT_H
#define MYMOUSEEVENT_H
#include "Common.h"
namespace my_master {


class MyMouseEvent : public my_master::MyEvent
{
public:
    MyMouseEvent();
    ~MyMouseEvent();
    ///////////////////////////////////////
    /// override myevent method
    int GetEventFd(){return m_fd;}
    EVENT_TYPE GetEventType(){return EVENT_TYPE::FILE;}
    uint32_t GetEpollEventType(){return EPOLLIN;}
    ///////////////////////////////////////
    /// need override method
    virtual void* CallBackFunc(MyEvent*);

    /*
     *  0: left button up
     *  1: left button down
     *  2: right button down
     */
    int GetMouseType();
    int GetRelX();
    int GetRelY();

private:
    int m_fd;
    char m_buf[3];
};
} // end namespace
#endif // MYMOUSEEVENT_H
