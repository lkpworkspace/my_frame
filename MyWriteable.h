#ifndef MYWRITEABLE_H
#define MYWRITEABLE_H
#include "Common.h"
#include "MyEvent.h"
#include "MyThread.h"
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
    sem_t CreateSem();
    sem_t SemFind(my_master::MyEvent*);
    void Writeable(struct epoll_event*);
private:
    std::map<my_master::MyEvent*,sem_t> m_members;
    int m_epollFd;
};

#endif // MYWRITEABLE_H
