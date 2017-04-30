#ifndef MYTALKCLIENT_H
#define MYTALKCLIENT_H
#include "MyFrame.h"
#include "MyUdp.h"
#include <string>
#include <map>
#include <semaphore.h>

using namespace my_master;
extern MyUdp* g_udp;
#define MYTALK_HEAD_SIZE 2
#define MYTALK_LOGIN 0x0001
/*  ack head 0x0002
 *      ack num:
 *          0x00 login is error
 *          0x01 login is ok
 *          0x02 ack get friend(client use)
 *          0x03 ack get friend repeat(client use)
 */
#define MYTALK_ACK 0x0002
#define MYTALK_FRIEND 0x0003
typedef struct mytalk_friend_t
{
    std::string account;
    std::string name;
    std::string mark;
    MyAddrInfo info;
}mytalk_friend_t;

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
    static void HandleFriend(MyAddrInfo info,char* buf, int len);
    ///////////////////////////////////////
    /// build method
    /// TODO...
    static char* BuildAck(char num, int *outlen);
private:
    static std::map<std::string, mytalk_friend_t*> m_friends;
    static std::string m_account;
    static std::string m_password;
    static std::mutex m_mutex;
    static sem_t m_sem;
    static MyAddrInfo m_server_info;
    static bool m_is_ok;
};

#endif // MYTALKCLIENT_H
