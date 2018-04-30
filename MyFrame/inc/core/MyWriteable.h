#ifndef MYWRITEABLE_H
#define MYWRITEABLE_H
#include "MyCommon.h"
#include "MyEvent.h"
#include "MyThread.h"
#include "MyControls.h"
#include <semaphore.h>
#include <map>

NS_MYFRAME_BEGIN

class MyWriteable : public MyThread
{
public:
    MyWriteable();
    virtual void Run();
    virtual void OnInit(){MyThread::OnInit();}
    virtual void OnExit(){MyThread::OnExit();}

    void WaitWriteable(myframe::MyEvent*);
    void AddWriteEvent(myframe::MyEvent*);
private:
public:
    sem_t* CreateSem();
    sem_t* SemFind(myframe::MyEvent*);
    void Writeable(struct epoll_event*, int count);
private:
    //std::map<my_master::MyEvent*,sem_t> m_members;
    MyMap<myframe::MyEvent*,sem_t*> m_members;
    int m_epollFd;
    int m_evc;
    struct epoll_event* m_ev;
};

NS_MYFRAME_END // end namespace
#endif // MYWRITEABLE_H
