#ifndef MYTALKSERVER_H
#define MYTALKSERVER_H
#include "MyFrame.h"
#include "MyUdp.h"
#include "MySqlite3.h"
#include <semaphore.h>
#include <map>

using namespace my_master;
#define MYTALK_HEAD_SIZE 2
#define MYTALK_LOGIN 0x0001
/*  ack head 0x0002
 *      ack num:
 *          0x00 login is error(client use handle)
 *          0x01 login is ok(client use handle)
 *          0x02 ack get friend ok(client use build)
 *          0x03 ack get friend repeat(client use build)
 */
#define MYTALK_ACK 0x0002
#define MYTALK_FRIEND 0x0003
#define MTTALK_MSG 0x0004

#define MYTALK_ONLINE 0x01
#define MYTALK_OFFLINE 0x00
typedef struct mytalk_friend_t
{
    std::string account;
    std::string name;
    std::string mark;
}mytalk_friend_t;

class MyTalkServer : public my_master::MyAllEvent
{
public:
    typedef struct member_t
    {
        std::string account;
        std::string name;
        std::string mark;
        uint8_t online;
        MyAddrInfo info;
    }member_t;

    MyTalkServer();

    virtual bool Event(MyEvent*);
    std::vector<mytalk_friend_t> GetFriends(std::string account);
    //////////////////////////////////////
    /// build msg
    char* BuildAck(char num, int *outlen);
    char* BuildFriend(std::string account,
                      std::string name,
                      std::string mark,
                      int* outlen);
    //////////////////////////////////////
    /// handle msg
    void HandleMsg(MyAddrInfo info,char* buf, int len);
    void HandleTalkMsg(MyAddrInfo info,char* buf, int len);
    void HandleLogin(MyAddrInfo info,char* buf, int len);
    void HandleAck(MyAddrInfo info, char* buf, int len);
private:
    sem_t m_sem;
    std::map<std::string,member_t> m_mem_status;
};

#endif // MYTALKSERVER_H
