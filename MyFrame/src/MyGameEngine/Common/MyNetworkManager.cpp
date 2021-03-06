#include "MyGameEngineShared.h"


void	MyNetworkManager::ProcessIncomingPackets(MyList *inList)
{
    mPacketQueue.Append(inList);

    ProcessQueuedPackets();
}

void	MyNetworkManager::ProcessQueuedPackets()
{
    MyNode* begin;
    MyNode* end;
    MyNode* temp;
    begin = mPacketQueue.Begin();
    end = mPacketQueue.End();

    for(;begin != end;)
    {
        temp = begin->next;
        // for server: begin is MyGameMsg pointer
        // for client: begin is MyInputStream pointer
        ProcessPacket((MyInputStream*)begin);
        mPacketQueue.Del(begin,false);
        begin = temp;
    }
}


void MyNetworkManager::AddToNetworkIdToGameObjectMap( MyGameObj* inGameObject )
{
    mNetworkIdToGameObjectMap[ inGameObject->GetNetworkId() ] = inGameObject;
}

void MyNetworkManager::RemoveFromNetworkIdToGameObjectMap( MyGameObj* inGameObject )
{
    mNetworkIdToGameObjectMap.erase( inGameObject->GetNetworkId() );
}
