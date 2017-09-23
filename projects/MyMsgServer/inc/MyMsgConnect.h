#ifndef __MYCONNECT_H__
#define __MYCONNECT_H__
#include "MyMsgCommon.h"
#include "MyTcp.h"

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

    /////////////////////////////////////////////////
    /// Ass func
    void MemberQuit(std::string name);
    std::string GetAccount();
    std::string Getpass();
    void InitAccountInfo(std::string id, std::string password);
    //////////////////////////////////////////////////
    /// Msg Process
    void Handle(const char* buf, int len);
    void HandleLogin(const char* buf, int len);
    void HandleErr(const char* buf, int len);
    void WelcomeMsg();
    //////////////////////////////////////////////////
    /// Msg Build
    char* BuildErr(unsigned short err_num, int* outlen);

    std::string m_id;               // id card
    std::string m_lv;               // is root or consumer
    std::string m_pass;
    std::string m_group;            // belong group
    std::string m_server;           // belong server
    std::string m_name;             // user name
private:
    std::mutex m_mutex;
    char* m_buf;
    int m_buf_size;
};



#endif
