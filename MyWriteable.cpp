#include "MyWriteable.h"

MyWriteable::MyWriteable()
{
    m_epollFd = epoll_create1(0);
    assert(m_epollFd != -1);
}

void MyWriteable::Run()
{

}

void MyWriteable::Writeable(struct epoll_event* evs)
{

}

void MyWriteable::AddWriteEvent(my_master::MyEvent* ev)
{
    //TODO...
}

sem_t MyWriteable::CreateSem()
{
    sem_t temp;
    int res = sem_init(&temp,0,0);
    assert(res != -1);
    return temp;
}

sem_t MyWriteable::SemFind(my_master::MyEvent* ev)
{

}

void MyWriteable::WaitWriteable(my_master::MyEvent* ev)
{
    sem_t temp = SemFind(ev);
    sem_wait(&temp);
}
