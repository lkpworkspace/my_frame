#include <list>

#include "MyFrame.h"
#include "GUtils.h"
#include "GMsg.h"
#include "IGClientProxy.h"
#include "IGMsgProcess.h"
#include "GMatchingManager.h"



/* call by GNetManager class */
void GMatchingManager::ProcessFunc(GMsg* inMsg)
{
    IGClientProxy* client = (IGClientProxy*)inMsg->GetClientProxy();
    unsigned short matching_action;

    inMsg->Read(matching_action, 16);

    switch(matching_action)
    {
    case EMA_MATCHING:
        client->SetState(IGClientProxy::EPS_MATCHING);
        mMatchingList.push_back(client->GetPlayerId());
        break;
    case EMA_CANCEL:
    default:
        return;
        break;
    }

    MatchingRule();
}

//TODO(lkp)
void GMatchingManager::MatchingRule()
{

    return;
}
