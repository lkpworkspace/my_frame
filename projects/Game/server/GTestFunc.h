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

void TestFunc()
{
#if 0
    char local_ip[16];
    MyNet::GetLocalIp("wlp2s0", local_ip);
    MyDebugPrint("local ip %s\n",local_ip);
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
