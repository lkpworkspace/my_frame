#ifndef COMM_H
#define COMM_H
#include "MyFrame.h"
#include "MySqlite3.h"

#define IP "127.0.0.1"
#define PORT 4399

#define MSG_HEAD_SIZE 2

enum ERR_MSG{
    ERR_OK,
    ERR_PASSWORD,
    ERR_ACCOUNT,
};

class MyMsgServer;
class MyMsgManager;
class MyMsgConnect;

void SetMsgServer(MyMsgServer* s);
MyMsgServer* GetMsgServer();

void SetMySqlite3(MySqlite3* s);
MySqlite3* GetMySqlite3();

#endif // COMM_H
