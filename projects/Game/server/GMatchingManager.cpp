#include <list>

#include "GServer.h"



/* call by GNetManager class */
void GMatchingManager::ProcessMsgs()
{
    GMsg* temp = nullptr;
    GMsg* msg = (GMsg*)mRecvMsgs.Begin();
    for(;msg != mRecvMsgs.End();)
    {
        temp = (GMsg*)msg->next;
        mRecvMsgs.Del(msg);

        MyDebugPrint("Process Matching Message\n");
        ProcessMsg(msg);

        FreeGMsg(msg);
        msg = temp;
    }
}

void GMatchingManager::ProcessMsg(GMsg* inMsg)
{
    IGClientProxy* client = inMsg->GetClientProxy();
    unsigned short matching_action;

    inMsg->Read(matching_action, 16);
    switch(matching_action)
    {
    case EMA_MATCHING:
        if(client)
        {
            client->SetState(IGClientProxy::EPS_MATCHING);
            mMatchingList.push_back(client->GetPlayerId());
        }
//        mMatchingList.push_back(100);
//        mMatchingList.push_back(200);
//        mMatchingList.push_back(300);
        /* 是否创建场景 */
        MatchingRule();
        break;
    case EMA_CANCEL:
    default:
        return;
        break;
    }
}


void GMatchingManager::MatchingRule()
{
    int player_id[3];
    if(mMatchingList.size() >= 3)
    {
        for(int i = 0; i < 3; ++i)
        {
            player_id[i] = mMatchingList.front();
            mMatchingList.pop_front();
        }
    }else
    {
        MyDebugPrint("Matching Queue Size: %d\n", mMatchingList.size());
        return;
    }
    GReplyMsg* re = BuildCreateSceneMsg(player_id);
    mReplicationMsg.AddTail(re);
    return;
}

GReplyMsg* GMatchingManager::BuildCreateSceneMsg(int* inPlayers)
{
    // create msg: send to GSceneManager
    GReplyMsg* re = nullptr;
    re = GetGReplyMsg();
    re->SetMsgTo(EMTO_MANAGER);
    re->SetMsgToId(EMT_SCENE);

    unsigned short msg_type = EMS_CREATE;
    re->Write(msg_type, 16);
    re->Write((int)inPlayers[0], 32);
    re->Write((int)inPlayers[1], 32);
    re->Write((int)inPlayers[2], 32);

    return re;
}








