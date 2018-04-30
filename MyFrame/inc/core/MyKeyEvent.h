#ifndef MYKEYEVENT_H
#define MYKEYEVENT_H
#include "Common.h"
#include "MyEvent.h"
namespace my_master {

class MyKeyEvent : public my_master::MyEvent
{
public:
    MyKeyEvent();
    virtual ~MyKeyEvent();
    /////////////////////////////////////////////////
    /// override MyEvent virtual method
    virtual int GetEventFd(){return m_fd;}
    virtual EVENT_TYPE GetEventType();     // maybe not most need
    virtual std::string GetClassType();
    virtual uint32_t GetEpollEventType();

    int GetKey();
protected:
    virtual void* CallBackFunc(MyEvent*);
private:
    std::string GetDevPath();
    int m_fd;
};

} // end namespace
#endif // MYKEYEVENT_H
