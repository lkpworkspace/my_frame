#ifndef MYWRITEABLE_H
#define MYWRITEABLE_H
#include "Common.h"
#include "MyEvent.h"
#include "MyThread.h"
#include "MyControls.h"
#include <semaphore.h>
#include <map>

class MyWriteable : public my_master::MyThread
{
public:
    MyWriteable();
    virtual void Run();
    virtual void OnInit(){MyThread::OnInit();}
    virtual void OnExit(){MyThread::OnExit();}

    void WaitWriteable(my_master::MyEvent*);
    void AddWriteEvent(my_master::MyEvent*);
private:
public:
    sem_t CreateSem();
    sem_t SemFind(my_master::MyEvent*);
    void Writeable(struct epoll_event*, int count);
private:
    //std::map<my_master::MyEvent*,sem_t> m_members;
    MyMap<my_master::MyEvent*,sem_t> m_members;
    int m_epollFd;
    int m_evc;
    struct epoll_event* m_ev;
};

#endif // MYWRITEABLE_H
