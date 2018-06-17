#include "GServer.h"

GServer::GServer()
{
    /**
     * 设置线程是帧循环的，而不是基于事件驱动的
     */
    // 调试阶段不使用帧循环驱动，而是用事件驱动(default)
    SetLoop(true);

    /**
     * 设置该线程只处理指定自己ID的事件
     */
    SetSpecifledEv(true);

    /**
     * 设置该工作线程的ID(其他线程靠此ID将消息发送过来)
     */
    SetTaskIdentifyWithName("GServer", GetIdentify());
}

void GServer::OnInit()
{
    MyTask::OnInit();
    printf("hello,world\n");
}


void GServer::Update(MyList *evs)
{
    // 消息分发处理函数
    GNetManager::sInstance->ProcessPacket(evs);
    // 消息回复函数(将消息发送到写线程，并返回)
    GNetManager::sInstance->SendOutgoingMsgs(this);
}
