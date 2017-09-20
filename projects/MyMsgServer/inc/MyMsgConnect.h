#ifndef __MYCONNECT_H__
#define __MYCONNECT_H__
#include "MyMsgCommon.h"
#include "MyTcp.h"
#include "MyUdp.h"

class MyMsgManager;
class MyMsgConnect : public my_master::MyEasyTcpSocket
{
    friend class MyMsgManager;
public:
    MyMsgConnect(int fd, sockaddr_in addr);
    ~MyMsgConnect();
    /////////////////////////////////////////////////
    /// override parent method
    int Frame(const char* buf, int len);
    std::string GetClassType(){return m_id;}

    void WelcomeMsg();
    std::string GetAccount();
    std::string Getpass();
    void Handle(const char* buf, int len);
private:
    std::string m_id;               // id card
    std::string m_lv;               // is root or consumer
    std::string m_group;            // belong group
    std::string m_server;           // belong server

    std::string m_name;             // user name
};



#endif
