#ifndef MYMSGSERVER_H
#define MYMSGSERVER_H
#include "MyTcp.h"
#include "MyMsgCommon.h"
#include "MyControls.h"

class MyMsgServer : public my_master::MyTcpServer, public MyCtrlObj
{
public:
    MyMsgServer();
    ~MyMsgServer();

    //////////////////////////////////////////////
    /// override parent method
    std::string GetClassType(){return "MyMsgServer";}
    void* CallBackFunc(MyEvent *);
protected:
private:
};

#endif // MYMSGSERVER_H
