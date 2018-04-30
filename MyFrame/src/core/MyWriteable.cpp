#include "../inc/MyWriteable.h"

USING_MYFRAME;

MyWriteable::MyWriteable()
{
    m_evc = 10;
    m_ev = (struct epoll_event*)malloc(sizeof(struct epoll_event) * m_evc);

    m_epollFd = epoll_create1(0);
    assert(m_epollFd != -1);
}

void MyWriteable::Run()
{
    int res = epoll_wait(m_epollFd,m_ev,m_evc,1000*60);
    Writeable(m_ev,res);
}


void MyWriteable::Writeable(struct epoll_event* evs, int count)
{
    sem_t *sem;
    for(int i = 0; i < count; ++i)
    {
        sem = SemFind((MyEvent*)evs[i].data.ptr);
        sem_post(sem);
    }
}

void MyWriteable::AddWriteEvent(myframe::MyEvent* ev)
{
    //TODO...
    struct epoll_event event;

    event.data.ptr = ev;
    event.events = EPOLLOUT | EPOLLET;
    m_members.Insert(ev,CreateSem());
    //epoll_ctl(m_epollFd,EPOLL_CTL_ADD,ev->GetEventFd(),&event);
}

sem_t *MyWriteable::CreateSem()
{
    sem_t* temp = (sem_t*)malloc(sizeof(sem_t));
    int res = sem_init(temp,0,0);
    assert(res != -1);
    return temp;
}

sem_t* MyWriteable::SemFind(myframe::MyEvent* ev)
{
    return m_members.Get(ev);
}

// call by user
void MyWriteable::WaitWriteable(myframe::MyEvent* ev)
{
    sem_t* temp = SemFind(ev);
    //if(temp)
        sem_wait(temp);
}
