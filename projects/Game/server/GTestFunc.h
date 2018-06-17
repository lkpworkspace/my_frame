#ifndef __GTestFunc_H__
#define __GTestFunc_H__

#include "GServer.h"

void Add(GMsg* inMsg, GReplyMsg* outMsg)
{
    outMsg = outMsg;
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
    std::string funcname = "ADD";
    int a = 100, b = 200;

    buildbuf.Write(msgtype, 16);
    buildbuf.Write(funcname);
    buildbuf.Write(a, 32);
    buildbuf.Write(b, 32);

    GMsg* msg = GetGMsg();
    msg->SetBuffer(buildbuf.GetBufferPtr(), buildbuf.GetByteLength() * 8);

    MyList msg_list;
    msg_list.AddTail(msg);
    netmanager->ProcessPacket(&msg_list);
}

void TestLogin(GNetManager* netmanager)
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

    MyList msg_list;
    msg_list.AddTail(msg);
    netmanager->ProcessPacket(&msg_list);
}

void TestMatchingMsgToSceneManager(GNetManager* netmanager)
{
    // 创建 匹配消息
    MyOutputStream buildbuf;
    unsigned short msgtype = EMT_MATCHING;
    unsigned short match_action = GMatchingManager::EMA_MATCHING;
    buildbuf.Write(msgtype, 16);
    buildbuf.Write(match_action, 16);
    GMsg* matching_msg = GetGMsg();
    matching_msg->SetBuffer(buildbuf.GetBufferPtr(), buildbuf.GetByteLength() * 8);

    MyList msg_list;
    msg_list.AddTail(matching_msg);
    // netmanager 处理匹配消息
    netmanager->ProcessPacket(&msg_list);

    // 发送到matchingmanager后， 发送创建场景消息到 scenemanager 打印输出消息
}

class A{
public:
    virtual void func1() = 0;
};

class B
{
public:
    virtual void func2()
    {
        printf("B func2\n");
    }
};

class D{
public:
    //virtual void func4() = 0;
};

class C : public A , public B
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
    virtual void func4()
    {
        printf("C func4\n");
    }
};

void TestFunc()
{
#if 0
    char local_ip[16];
    MyNet::GetLocalIp("wlp2s0", local_ip);
    MyDebugPrint("local ip %s\n",local_ip);
    /// 并行继承时，C++会在孩子对象中创建2个vtable
    /// 指针经过两次类型转换后 会出现虚函数调用错误的情况。
    C c;
    void* x = &c;
    B* b = (B*)x;
    b->func2();
#else
    GNetManager netmanager;
    GRPCManager rpcmanager;
    GLoginManager loginmanager;
    GMatchingManager matchingmanager;
    GSceneManager scenemanager;

    netmanager.RegMsgProcesssManager(EMT_RPC,&rpcmanager);
    netmanager.RegMsgProcesssManager(EMT_LOGIN,&loginmanager);
    netmanager.RegMsgProcesssManager(EMT_MATCHING,&matchingmanager);
    netmanager.RegMsgProcesssManager(EMT_SCENE,&scenemanager);

//    TestRPC(&netmanager, &rpcmanager);

//    TestLogin(&netmanager);

    /* MatchingManager发送消息到SceneManager */
    TestMatchingMsgToSceneManager(&netmanager);


#endif
}

#endif





































///////////////////////////////////////////////////////////////////////////////////////////

//    netmanager.RegMsgProcessFunc(EMT_RPC,
//                                 &rpcmanager,
//                                 PROCESS_FUNC(GRPCManager::ProcessFunc),
//                                 REPLY_FUNC(GRPCManager::GetReplyMsg));
//    netmanager.RegMsgProcessFunc(EMT_LOGIN,
//                                 &loginmanager,
//                                 PROCESS_FUNC(GLoginManager::ProcessFunc),
//                                 REPLY_FUNC(GLoginManager::GetReplyMsg));

//    netmanager.RegMsgProcessFunc(EMT_MATCHING,
//                                 &matchingmanager,
//                                 PROCESS_FUNC(GMatchingManager::ProcessFunc),
//                                 REPLY_FUNC(GMatchingManager::GetReplyMsg));

//    netmanager.RegMsgProcessFunc(EMT_SCENE,
//                                 &scenemanager,
//                                 PROCESS_FUNC(GSceneManager::ProcessFunc),
//                                 REPLY_FUNC(GSceneManager::GetReplyMsg));
