#include "GServer.h"

void GLoginManager::ProcessMsgs()
{
    GMsg* temp = nullptr;
    GMsg* msg = (GMsg*)mRecvMsgs.Begin();
    for(;msg != mRecvMsgs.End();)
    {
        temp = (GMsg*)msg->next;
        mRecvMsgs.Del(msg);

        MyDebugPrint("Process Login Message\n");
        ProcessMsg(msg);

        FreeGMsg(msg);
        msg = temp;
    }
}

void GLoginManager::ProcessMsg(GMsg* inMsg)
{
//    std::string account;
//    std::string password;

    IGClientProxy* c = inMsg->GetClientProxy();

    if(c == nullptr) return;
    /* set login state */
    c->SetState(IGClientProxy::EPS_LOGIN);
    MyDebugPrint("Player %d Login\n", c->GetPlayerId());

    /* build reply msg */
    unsigned short msg_type = EMT_LOGIN;
    unsigned short login_state = ELS_OK;
    unsigned int player_id = c->GetPlayerId();

    GReplyMsg* reply = GetGReplyMsg();
    reply->SetClientProxy(c);
    reply->Write(msg_type,16);
    reply->Write(login_state,16);
    reply->Write(player_id,32);
    mReplyMsgs.AddTail(reply);
}

MyList* GLoginManager::GetReplyMsg()
{
    return &mReplyMsgs;
}
