#ifndef MYTALKCLIENT_H
#define MYTALKCLIENT_H
#include "MyFrame.h"

using namespace my_master;

class MyTalkClient : public my_master::MyAllEvent
{
public:
    MyTalkClient();

    // override MyAllEvent method
    virtual bool Event(MyEvent* ev);

    ///////////////////////////////////////
    /// handle method
    /// TODO...

    ///////////////////////////////////////
    /// build method
    /// TODO...

};

#endif // MYTALKCLIENT_H
