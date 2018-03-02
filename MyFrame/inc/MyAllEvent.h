#ifndef MYALLEVENT_H
#define MYALLEVENT_H
#include "MyCommon.h"
#include "MyList.h"
#include <mutex>

class MyEvent;
class MyNode;
class MyList;

NS_MYFRAME_BEGIN

class MyAllEvent : public MyNode
{
public:
    MyAllEvent();
    virtual ~MyAllEvent();
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

NS_MYFRAME_END // end namespace
#endif // MYALLEVENT_H
