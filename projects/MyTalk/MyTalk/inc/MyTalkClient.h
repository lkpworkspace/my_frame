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
#define MYTALK_DEBUG 1
#define MYTALK_ACK 0x0002
#define MYTALK_FRIEND 0x0003
#define MYTALK_MSG 0x0004

#define MYTALK_ONLINE 0x01
#define MYTALK_OFFLINE 0x00
typedef struct mytalk_friend_t
{
    std::string account;
    std::string name;
    std::string mark;
    /*  0x00 offline;
     *  0x01 online;
     */
    uint8_t online;
    MyAddrInfo info;
}mytalk_friend_t;

class MyTalkClient : public my_master::MyAllEvent
{
public:
    MyTalkClient();

    // override MyAllEvent method
    virtual bool Event(MyEvent* ev);
    static void Init();
    static void UnInit(){}
    static void SendLogin(std::string account, std::string password);
    static bool CheckLogin();
    static int SendTalkMsg(std::string to, std::string msg);
    ///////////////////////////////////////
    /// handle method
    /// TODO...
    static void HandleMsg(MyAddrInfo info, char* buf, int len);
    static bool HandleLogin(char* buf, int len);
    static void HandleFriend(MyAddrInfo info,char* buf, int len);
    static void HandleTalkMsg(MyAddrInfo info, char* buf, int len); // TODO...
    ///////////////////////////////////////
    /// build method
    /// TODO...
    static char* BuildAck(char num, int *outlen);
    static char* BuildTalkMsg(std::string src, std::string dst, std::string msg, int* outlen);

    static std::map<std::string, mytalk_friend_t*> m_friends;
    static std::mutex m_mutex;
    static std::string m_account;
private:
    static std::string m_password;

    static sem_t m_sem;
    static MyAddrInfo m_server_info;
    static bool m_is_ok;
};

#endif // MYTALKCLIENT_H
