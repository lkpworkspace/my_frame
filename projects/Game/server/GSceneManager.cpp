#include "GServer.h"


GSceneManager::GSceneManager()
    :mNewSceneId(0)
{
}

GSceneManager::~GSceneManager()
{}


/* server/client 消息处理回调函数 */
void GSceneManager::ProcessMsgs()
{
    GMsg* temp = nullptr;
    GMsg* msg = (GMsg*)mRecvMsgs.Begin();
    for(;msg != mRecvMsgs.End();)
    {
        temp = (GMsg*)msg->next;
        mRecvMsgs.Del(msg);

        MyDebugPrint("Process SceneManager Message\n");
        ProcessMsg(msg);

        FreeGMsg(msg);
        msg = temp;
    }

    // TODO(lkp) ...
    // 暂时使用这种更新方式(每次都更所有的场景，不管有没有消息)
    // 之后再说
    auto iter = mScenesMap.begin();
    for(;iter != mScenesMap.end(); iter++)
    {
        UpdateScene(iter->first);
    }
}

void GSceneManager::ProcessMsg(GMsg* inMsg)
{
    int i = 0;
    unsigned short msg_type;
    int scene_id;
    int players_id[3];
    GScene* scene = nullptr;

    inMsg->Read(msg_type,16);
    switch (msg_type) {
    case EMS_CREATE:
        for(i = 0; i < 3; ++i)
        {
            inMsg->Read(players_id[i], 32);
        }
        CreateScene(players_id, 3);
        break;
    case EMS_UPDATE:
        scene_id = inMsg->GetClientProxy()->GetSceneId();
        scene = mScenesMap[scene_id];
        scene->WorldAppendMsg(inMsg);
        break;
    case EMS_DESTORY:

        break;
    default:
        break;
    }
}

/* 创建并初始化一个场景 */
void GSceneManager::CreateScene(int* inPlayersId, int inPlayerCount)
{
    MyDebugPrint("Create Scene %d\n", mNewSceneId);
    // New GScene, Init World, Add to mScenesMap
    GScene* scene = new GScene(mNewSceneId, inPlayersId, inPlayerCount);
    scene->WorldInit();
    mScenesMap[mNewSceneId] = scene;
    mNewSceneId++;
}

void GSceneManager::UpdateScene(int inSceneId)
{
    GScene* scene = mScenesMap[inSceneId];
    MyDebugPrint("Update Scene %d\n", scene->GetSceneId());
    scene->WorldUpdate();
}
