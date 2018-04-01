#ifndef MyEasyTcpClient_H
#define MyEasyTcpClient_H

#include <stdint.h>

#include "MyNet.h"
#include "MyIOStream.h"
#include "MyDataParser.h"

class MySimpleTcpClient
{
public:
    MySimpleTcpClient();
    MySimpleTcpClient(MyAddrInfo& inAddrInfo);
    virtual ~MySimpleTcpClient();

    int Connect();
    int SetReuseSock();
    void SetAddrInfo(MyAddrInfo& info);

    int32_t EasySend(const char* inData, size_t inLen);
    void EasyRecv();
    int GetFrame(MyInputStream& outInputStream);
protected:
    int32_t Send(const char* inData, size_t inLen);
    int32_t Recv(void* inData, size_t inLen);
    int Socket();
    int SetNonBlocking(bool b);
private:
    bool Init();
    void Exit();
private:
    MyAddrInfo mAddrInfo;
    MyDataParser mParser;
    SOCKET mSock;
};

#endif
