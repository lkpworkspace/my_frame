#include "MyAllEvent.h"
using namespace my_master;

MyList MyAllEvent::g_event;
std::mutex MyAllEvent::g_event_mutex;

void MyAllEvent::BoardEvent(MyEvent* ev)
{
    g_event_mutex.lock();
    MyNode* begin = g_event.Begin();
    MyNode* end = g_event.End();
    for(;begin != end;)
    {
        MyAllEvent* ae = (MyAllEvent*)begin;
        ae->Event(ev);
        begin = begin->next;
    }
    g_event_mutex.unlock();
}

MyAllEvent::MyAllEvent()
    :m_parent(nullptr)
{
    // TODO...
    g_event.AddTail(this);
}

MyAllEvent::~MyAllEvent()
{}

void MyAllEvent::AddChild(MyAllEvent* ev)
{
    if(ev->prev != nullptr)
        g_event.Del(ev,false);
    m_childs.AddTail(ev);
    ev->m_parent = (MyNode*)this;
}

void MyAllEvent::DetachFromParent()
{
    if(prev != NULL)
        g_event.Del((MyNode*)this,false);
    m_parent = nullptr;
}

MyList* MyAllEvent::GetChilds()
{
    return &m_childs;
}

MyNode* MyAllEvent::GetParent()
{
    return m_parent;
}
