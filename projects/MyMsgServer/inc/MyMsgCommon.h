#ifndef COMM_H
#define COMM_H
#include "MyFrame.h"
#include "MySqlite3.h"
#include "MyControls.h"
#if 0
#define IP "127.0.0.1"
#else
#define IP "172.16.212.139"
#endif
#define PORT 4399

#define MSG_HEAD_SIZE 2

typedef enum EnumMsgCode{
    ERR_OK,                         ///< OK
    ERR_PASSWORD,                   ///< 密码错误
    ERR_NOACCOUNT,                  ///< 帐号错误
    ERR_ALREADYLOGIN,               ///< 已经登录
    ERR_NOMEMBER,                   ///< 没有该帐号(要发送的帐号)
    ERR_NOFUNC,                     ///< 没有此功能
    ERR_FORMAT,                     ///< 错误的消息格式
    ERR_REPEATREQ,                  ///<
}EnumMsgCode_t;

typedef enum EnumMsgRequest{
    REQ_NONE,
    REQ_LOGIN,
    REQ_SINMSG,
    REQ_ALLFRIEND,
    REQ_ONLINEFRIEND,
    REQ_SEARCHFRIENDID
}EnumMsgRequest_t;

typedef struct _AccountInfo
{
    std::string m_id;               ///< id
    std::string m_lv;               ///< is root or customer
    std::string m_pass;             ///< password
    std::string m_group;            ///< belong group
    std::string m_server;           ///< belong server
    std::string m_name;             ///< user name
}AccountInfo_t;

typedef struct _OLFriend{
    int index;
    int total;
}OLFriend_t;

typedef struct _ALLFriend{
    int index;
    int total;
}ALLFriend_t;

class MyMsgServer;
class MyMsgManager;
class MyMsgConnect;

void SetMsgServer(MyMsgServer* s);
MyMsgServer* GetMsgServer();

void SetMySqlite3(MySqlite3* s);
MySqlite3* GetMySqlite3();

unsigned char CheckSum(const unsigned char* buf, int len);

std::string GetAccount();
std::string Getpass();

#endif // COMM_H
