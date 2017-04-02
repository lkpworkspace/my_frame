#ifndef MYALLEVENT_H
#define MYALLEVENT_H
#include "Common.h"
namespace my_master {
class MyEvent;
class MyTree;
class MyNode;
class MyList;
class MyAllEvent : public my_master::MyNode
{
public:
    MyAllEvent();
    ~MyAllEvent();
    ///////////////////////////////////////
    /// need override method
    virtual bool Event(MyEvent*) = 0;

    void AddChild(MyAllEvent*);
    void DetachFromParent();
    MyList* GetChilds();
    MyNode* GetParent();

    static void BoardEvent(MyEvent*);
    static MyList g_event;
    static std::mutex g_event_mutex;
private:
    MyList m_childs;
    MyNode* m_parent;
};

} // end namespace
#endif // MYALLEVENT_H
