#ifndef MYNORMALEVENT_H
#define MYNORMALEVENT_H
#include "MyEvent.h"

NS_MYFRAME_BEGIN

class MyNormalEvent : public myframe::MyEvent
{
public:
    MyNormalEvent();
    virtual ~MyNormalEvent();
    ////////////////////////////////////////////// override MyEvent method
    int GetEventFd(){return m_msgFd[1];}
    EVENT_TYPE GetEventType(){return EVENT_TYPE::EV_FILEFD;}
    uint32_t GetEpollEventType(){ return EPOLLIN; }
    virtual void* CallBackFunc(MyEvent *);

    int Work();                                        // invoke by child class
    void SetFunc(void(*func)(myframe::MyEvent*)){m_event_func = func;}
private:
    int RecvMsg(char *buf, int len);                  // invoke by myself
    int SendMsg(const char* buf, int len);             // invoke by child class
    int CreateSockPair();   // communication between thread
    void Close(){close(m_msgFd[0]);close(m_msgFd[1]);}
    int m_msgFd[2];
    uint8_t m_msgBuf[MSG_LEN];
    void (*m_event_func)(myframe::MyEvent* ev);
};

NS_MYFRAME_END // end namespace
#endif // MYNORMALEVENT_H
