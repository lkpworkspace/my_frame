#ifndef MYEVENT_H
#define MYEVENT_H

#ifdef linux
#include <sys/epoll.h>
#endif

#include "MyCommon.h"
#include "MyList.h"
#include "MyObj.h"

NS_MYFRAME_BEGIN

class MyEvent : public MyNode, public MyObj
{
    friend class MyTask;
public:
    enum EVENT_TYPE{
        EV_TASKFD,
        EV_FILEFD,
        EV_SOCKFD,
        EV_MSG,
        EV_NONE
    };

public:
    MyEvent();
    virtual ~MyEvent();

    /**
     * 获得当前事件的文件描述符
     */
    virtual int GetEventFd(){ return -1; }

    /**
     * 监听的是文件描述符的写事件还是读事件
     * 一般是读事件(EPOLLIN)
     */
    virtual uint32_t GetEpollEventType(){ return -1; }

    /**
     * GetEventType() - 必须重写的函数
     * 区分 线程事件 还是其他 文件描述符 事件
     */
    virtual EVENT_TYPE GetEventType() = 0;

    /**
     * 没啥用，仅用作标记当前对象的名字
     */
    virtual std::string GetClassType(){ return GetObjName();}    // maybe not most need

    /**
     * 指定处理事件的线程标识
     */
    virtual void SetSendIdentify(int identify){ m_send_identify = identify; }
    virtual int GetSendIdentify(){ return m_send_identify; }

    /**
     * 发送事件队列,一般将读到的数据创建成一个MyEvent子类事件发送给相应的线程处理
     * 该发送事件只需要指定线程标识(SetSendTag)就可以，不需要重写其他MyEvent虚函数
     */
    MyList* GetSendEvQue(){return &m_ev_send_que;}
    void AddSendEv(MyEvent* ev) { m_ev_send_que.AddTail(ev);}

protected:
    /**
     * 事件的回调函数
     */
    virtual void* CallBackFunc(MyEvent*) = 0;

private:

    /* 回调函数之后产生的普通事件队列 */
    MyList m_ev_send_que;                 // send event queue

    /* 标识该事件是否由指定线程处理(-1: 所有线程都可以处理) */
    int m_send_identify;
};

NS_MYFRAME_END // end namespace
#endif // MYEVENT_H
