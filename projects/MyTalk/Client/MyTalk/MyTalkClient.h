#ifndef MYTALKCLIENT_H
#define MYTALKCLIENT_H
#include "MyFrame.h"
#include "MyUdp.h"
#include <string>
#include <semaphore.h>

using namespace my_master;
extern MyUdp* g_udp;
#define MYTALK_HEAD_SIZE 2
#define MYTALK_LOGIN 0x0001
#define MYTALK_ACK 0x0002
class MyTalkClient : public my_master::MyAllEvent
{
public:
    MyTalkClient();

    // override MyAllEvent method
    virtual bool Event(MyEvent* ev);
    static void Init();
    static void SendLogin(std::string account, std::string password);
    static bool CheckLogin();
    ///////////////////////////////////////
    /// handle method
    /// TODO...
    static void HandleMsg(MyAddrInfo info, char* buf, int len);
    static bool HandleLogin(char* buf, int len);
    ///////////////////////////////////////
    /// build method
    /// TODO...

private:
    static std::string m_account;
    static std::string m_password;
    static std::mutex m_mutex;
    static sem_t m_sem;
    static MyAddrInfo m_server_info;
    static bool m_is_ok;
};

#endif // MYTALKCLIENT_H
