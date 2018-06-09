#include "GServer.h"

void GLoginManager::ProcessFunc(GMsg* inMsg)
{
    std::string account;
    std::string password;

    GClientProxy* c = (GClientProxy*)inMsg->GetClientProxy();
    IGClientProxy* ic = (IGClientProxy*)inMsg->GetClientProxy();
    MyEvent* e = (MyEvent*)inMsg->GetClientProxy();

    if(c == nullptr) return;
    e->GetEventFd();
    ic->SetPlayerId(mNewPlayerId);
    ic->SetState(IGClientProxy::EPS_LOGIN);

    /* set login state, assign player id */
    c->SetPlayerId(mNewPlayerId);
    c->SetState(IGClientProxy::EPS_LOGIN);

    /* build reply msg */
    unsigned short msg_type = EMT_LOGIN;
    unsigned short login_state = ELS_OK;
    unsigned int player_id = mNewPlayerId;

    GReplyMsg* reply = GetGReplyMsg();
    reply->SetClientProxy(c);
    reply->Write(msg_type,16);
    reply->Write(login_state,16);
    reply->Write(player_id,32);
    mReplyMsgs.AddTail(reply);

    mNewPlayerId++;
}

MyList* GLoginManager::GetReplyMsg()
{
    return &mReplyMsgs;
}
