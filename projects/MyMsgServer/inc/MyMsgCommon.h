#ifndef COMM_H
#define COMM_H
#include "MyFrame.h"
#include "MySqlite3.h"
#include "MyControls.h"

#define IP "127.0.0.1"
#define PORT 4399

#define MSG_HEAD_SIZE 2

typedef enum EnumMsgCode{
    ERR_OK,
    ERR_PASSWORD,
    ERR_NOACCOUNT,
    ERR_NOMEMBER
}EnumMsgCode_t;

typedef enum EnumMsgRequest{
    REQ_ALLFRIEND,
    REQ_ONLINEFRIEND,
    REQ_SEARCHFRIENDID
}EnumMsgRequest_t;

typedef struct _AccountInfo
{
    std::string m_id;               ///< id card
    std::string m_lv;               ///< is root or consumer
    std::string m_pass;
    std::string m_group;            ///< belong group
    std::string m_server;           ///< belong server
    std::string m_name;             ///< user name
}AccountInfo_t;

class MyMsgServer;
class MyMsgManager;
class MyMsgConnect;

void SetMsgServer(MyMsgServer* s);
MyMsgServer* GetMsgServer();

void SetMySqlite3(MySqlite3* s);
MySqlite3* GetMySqlite3();

#endif // COMM_H
