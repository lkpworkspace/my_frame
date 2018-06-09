#include "GServer.h"

int GClientProxy::Frame(const char* buf, int len)
{
    // 客户端退出
    if(len == 0)
    {
        //TODO(lkp) 向LoginManager发送退出消息
        MyDebugPrint("Client Quit\n");
        return false;
    }
    if(len < 2)
    {
        MyDebugPrint("Message Length %d\n",len);
        return false;
    }
    // 过滤消息
    unsigned short msg_type;
    GMsg* msg = GetGMsg();
    msg->SetBuffer(buf, len * 8);
    msg->Read(msg_type,16);
    FreeGMsg(msg);
    bool isOk = true;
    switch(msg_type)
    {
    case EMT_LOGIN:
        isOk = (mPlayerState == EPS_CONNECT);
        break;
    case EMT_MATCHING:
        isOk = (mPlayerState == EPS_LOGIN);
        break;
    case EMT_INPUT:
        isOk = (mPlayerState == EPS_FINGHTING);
        break;
    case EMT_RPC:
        break;
    case EMT_WORLD_STATE:break;
    default:
        MyDebugPrint("Unkonwn Message Type\n");
        break;
    }
    if(!isOk)
    {
        MyDebugPrint("Message Has Filter\n");
        return true;
    }

    // 传递消息
    GMsg* msg2 = GetGMsg();
    msg2->SetSendIdentify(GetTaskIdentify("GServer"));
    msg2->SetBuffer(buf, len * 8);
    msg2->SetClientProxy(this);
    AddSendEv(msg2);

    return true;
}
