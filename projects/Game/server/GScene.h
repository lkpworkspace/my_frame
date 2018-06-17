#ifndef __GScene_H__
#define __GScene_H__

class GMsg;
class MyList;
class GScene
{
public:
    GScene(int inSceneId, int* inPlayersId, int inPlayerCount)
        :mSceneId(inSceneId),
          mPlayerCount(inPlayerCount)
    {
        InitPlayersMap(inSceneId, inPlayersId, inPlayerCount);
    }
    virtual ~GScene(){}

    // 设置世界
    void WorldInit();
    void WorldAppendMsg(GMsg* inMsg){ mRecvMsgs.AddTail(inMsg); }
    void WorldUpdate();
    void WorldDestory();

    // 获得场景ID
    int GetSceneId() { return mSceneId; }

protected:
    void InitPlayersMap(int inSceneId, int* inPlayersId, int inPlayerCount);
private:
    /* 创建的场景ID */
    int mSceneId;
    /* 玩家ID列表 */
    std::unordered_map<int,GClientProxy*> mPlayersMap;
    /* 玩家数量 */
    int mPlayerCount;
    /* 消息缓存列表 */
    MyList mRecvMsgs;
};

#endif
