#ifndef MYSERVER_H
#define MYSERVER_H
#include "MyFrame.h"
#include "MyControls.h"
#include "MyCapConnect.h"

using namespace my_master;

class MyServer : public my_master::MyAllEvent, public MyCtrlObj
{
public:
    MyServer();
    bool Event(MyEvent* ev);


    std::string GetAccount();
    std::string Getpass();
};

#endif // MYSERVER_H
