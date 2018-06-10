#ifndef __GTestFunc_H__
#define __GTestFunc_H__

#include "GServer.h"

void Add(GMsg* inMsg, GMsg* outMsg)
{
    int a,b;
    inMsg->Read(a,32);
    inMsg->Read(b,32);
    printf("Add %d\n", a + b);
}

void TestRPC(GNetManager* netmanager, GRPCManager* rpcmanager)
{
    rpcmanager->RegRPCFunc("ADD",Add);

    MyOutputStream buildbuf;
    unsigned short msgtype = EMT_RPC;
    // func name
    std::string funcname = "ADD";
    // func arg
    int a = 100, b = 200;
    buildbuf.Write(msgtype, 16);
    buildbuf.Write(funcname);
    buildbuf.Write(a, 32);
    buildbuf.Write(b, 32);
    GMsg* msg = GetGMsg();
    msg->SetBuffer(buildbuf.GetBufferPtr(), buildbuf.GetByteLength() * 8);

    netmanager->ProcessPacket(msg);
}

void TestLogin(GNetManager* netmanager, GLoginManager* loginmanager)
{
    unsigned short msgtype = EMT_LOGIN;
    std::string account = "lkp";
    std::string password = "123456";

    MyOutputStream buildbuf;
    buildbuf.Write(msgtype);
    buildbuf.Write(account);
    buildbuf.Write(password);

    GMsg* msg = GetGMsg();
    msg->SetBuffer(buildbuf.GetBufferPtr(), buildbuf.GetByteLength() * 8);
    netmanager->ProcessPacket(msg);
}

class A{
public:
    virtual void func1() = 0;
};

class B : public A
{
public:
    virtual void func1()
    {
        printf("B func1\n");
    }
    virtual void func2()
    {
        printf("B func2\n");
    }
};

class C : public B
{
public:
    virtual void func2()
    {
        printf("C func2\n");
    }
    virtual void func1()
    {
        printf("C func1\n");
    }
    virtual void func3()
    {
        printf("C func3\n");
    }
};

void TestFunc()
{
#if 1
    char local_ip[16];
    MyNet::GetLocalIp("wlp2s0", local_ip);
    MyDebugPrint("local ip %s\n",local_ip);

    /// 指针经过两次类型转换后 会出现虚函数调用错误的情况。
    C c;
    void* x = &c;
    B* b = (B*)x;
    b->func1();
#else
    GNetManager netmanager;
    GRPCManager rpcmanager;
    GLoginManager loginmanager;
    // TODO(lkp) test matching,

    netmanager.RegMsgProcessFunc(EMT_RPC,
                                 &rpcmanager,
                                 PROCESS_FUNC(GRPCManager::ProcessFunc),
                                 REPLY_FUNC(GRPCManager::GetReplyMsg));
    netmanager.RegMsgProcessFunc(EMT_LOGIN,
                                 &loginmanager,
                                 PROCESS_FUNC(GLoginManager::ProcessFunc),
                                 REPLY_FUNC(GLoginManager::GetReplyMsg));

    TestRPC(&netmanager, &rpcmanager);

    TestLogin(&netmanager, &loginmanager);


#endif
}

#endif
