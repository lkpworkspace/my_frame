#ifdef GSERVER
#include "GServer.h"
#endif

static MyMsgPool* g_msgpool = nullptr;
#ifdef GSERVER
GRPCManager* g_rpc = nullptr;
GLoginManager* g_login = nullptr;
GMatchingManager* g_match = nullptr;
GSceneManager* g_scenes = nullptr;
#else
#endif
void GGlobalInit()
{
#ifdef GSERVER
    /* init MyMsgPool class */
    if(g_msgpool == nullptr)
        g_msgpool = new MyMsgPool();
    g_msgpool->RegMsg("GMsg", &GMsg::StaticCreate);
    g_msgpool->RegMsg("GReplyMsg", &GReplyMsg::StaticCreate);

    /* init GNetManager */
    GNetManager::StaticInit();

    /* registe message manager */
    g_rpc = new GRPCManager();
    GNetManager::sInstance->RegMsgProcesssManager(EMT_RPC, g_rpc);

    g_login = new GLoginManager();
    GNetManager::sInstance->RegMsgProcesssManager(EMT_LOGIN, g_login);

    g_match = new GMatchingManager();
    GNetManager::sInstance->RegMsgProcesssManager(EMT_MATCHING, g_match);

    g_scenes = new GSceneManager();
    GNetManager::sInstance->RegMsgProcesssManager(EMT_SCENE, g_scenes);
#else

#endif
}

GMsg* GetGMsg()
{
    return (GMsg*)g_msgpool->Get("GMsg");
}

void FreeGMsg(GMsg *inMsg)
{
    g_msgpool->Free(inMsg);
}

GReplyMsg* GetGReplyMsg()
{
    return (GReplyMsg*)g_msgpool->Get("GReplyMsg");
}

void FreeGReplyMsg(GReplyMsg* inMsg)
{
    g_msgpool->Free(inMsg);
}





















































///////////////////////////////////////////////////////////////////////////////////////////////
//    GNetManager::sInstance->RegMsgProcessFunc(EMT_LOGIN, g_login,
//                                              PROCESS_FUNC(GLoginManager::ProcessFunc),
//                                              REPLY_FUNC(GLoginManager::GetReplyMsg));

//    GNetManager::sInstance->RegMsgProcessFunc(EMT_RPC, g_rpc,
//                                              PROCESS_FUNC(GRPCManager::ProcessFunc),
//                                              REPLY_FUNC(GRPCManager::GetReplyMsg));
