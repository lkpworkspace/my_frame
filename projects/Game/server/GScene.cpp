#include "GServer.h"

void GScene::InitPlayersMap(int inSceneId, int* inPlayersId, int inPlayerCount)
{
    GClientProxy* client = nullptr;
    for(int i = 0; i < inPlayerCount; ++i)
    {
        client = GClientProxy::GetClient(inPlayersId[i]);
        if(client)
        {
            mPlayersMap[inPlayersId[i]] = client;
            client->SetSceneId(inSceneId);
            client->SetState(IGClientProxy::EPS_FINGHTING);
        }
        else
            MyDebugPrint("Client is nullptr\n");
    }
}

void GScene::WorldInit()
{
    MyDebugPrint("World %d Init\n", mSceneId);
}

void GScene::WorldUpdate()
{
    if(mRecvMsgs.IsEmpty()) return;
    MyDebugPrint("World %d Update\n", mSceneId);
}

void GScene::WorldDestory()
{
    MyDebugPrint("World %d Destored\n", mSceneId);
}
