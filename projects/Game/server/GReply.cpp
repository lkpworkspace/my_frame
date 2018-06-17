#include "GServer.h"


GReply::GReply(){
    /**
     * 设置线程是基于事件驱动的
     */
    SetLoop(false);

    /**
     * 设置该线程只处理指定自己ID的事件
     */
    SetSpecifledEv(true);

    /**
     * 设置该工作线程的ID(其他线程靠此ID将消息发送过来)
     */
    SetTaskIdentifyWithName("GReply", GetIdentify());
}

/* 发送消息到客户端 */
void GReply::Update(MyList *evs)
{
    MyNode* begin = nullptr;
    MyNode* end = nullptr;
    MyNode* temp = nullptr;
    GClientProxy* client = nullptr;
    GReplyMsg* msg = nullptr;
    begin = evs->Begin();
    end = evs->End();

    for(;begin != end;)
    {
        temp = begin->next;
        msg = (GReplyMsg*)begin;
        evs->Del(begin);
        client = (GClientProxy*)msg->GetClientProxy();
        MyDebugPrint("Player [%d] Write Message To Client\n", client->GetPlayerId());
        client->EasyWrite(msg->GetBufferPtr(), msg->GetByteLength());
        FreeGReplyMsg(msg);
        begin = temp;
    }
}

