#ifndef MYTALKSERVER_H
#define MYTALKSERVER_H
#include "MyFrame.h"
#include "MyUdp.h"
#include "MySqlite3.h"
using namespace my_master;
#define MYTALK_HEAD_SIZE 2
#define MYTALK_LOGIN 0x0001
#define MYTALK_ACK 0x0002
class MyTalkServer : public my_master::MyAllEvent
{
public:
    MyTalkServer();

    virtual bool Event(MyEvent*);
    //////////////////////////////////////
    /// build msg
    char* BuildAck(char num, int *outlen);
    //////////////////////////////////////
    /// handle msg
    void HandleMsg(MyAddrInfo info,char* buf, int len);
    void HandleLogin(MyAddrInfo info,char* buf, int len);
};

#endif // MYTALKSERVER_H
