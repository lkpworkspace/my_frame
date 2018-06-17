#ifndef __GSceneManager_H__
#define __GSceneManager_H__


/**
 * @brief The GSceneManager class
 *      注册到NetManager类中
 *      处理消息，创建场景， 回复消息到Gserver中去
 */
#include"GServer.h"
class GScene;
class GSceneManager : public IGMsgProcess
{
public:
    GSceneManager();
    virtual ~GSceneManager();

    virtual void AppendMsg(GMsg* inMsg) override { mRecvMsgs.AddTail(inMsg); }

    /* server/client 消息处理回调函数 */
    virtual void ProcessMsgs();

    /* server/client 消息处理完后获得的回复信息 */
    virtual MyList* GetReplyMsg(){ return &mReplyMsgsList; }

protected:
    void ProcessMsg(GMsg* inMsg);
    /* 创建并初始化一个场景 */
    void CreateScene(int *inPlayersId, int inPlayerCount);
    /* 更新每个场景 */
    void UpdateScene(int inSceneId);
private:
    /* 所有创建的场景 */
    std::unordered_map<int,GScene*> mScenesMap;
    /* 每个场景的回复消息列表 */
    MyList mReplyMsgsList;
    /* 获得的消息列表 */
    MyList mRecvMsgs;
    /* 场景分配的ID号(每创建一个场景增加1) */
    int mNewSceneId;
};

#endif
