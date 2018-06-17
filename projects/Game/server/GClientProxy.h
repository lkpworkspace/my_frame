#ifndef __GClientProxy_H__
#define __GClientProxy_H__

#include "IGClientProxy.h"

class GClientProxy : public myframe::MyEasyTcpSocket, public IGClientProxy
{
public:
    GClientProxy(int fd, sockaddr_in addr)
        :MyEasyTcpSocket(fd, addr),
          mPlayerState(EPS_CONNECT),
          mPlayerId(mNewPlayerId++),
          mGameThreadId(-1)
    {
        mClientsMap[mPlayerId] = this;
    }

    virtual ~GClientProxy()
    {
        mClientsMap.erase(mPlayerId);
    }

    /* override MyEasyTcpSocket method */
    virtual int Frame(const char* buf, int len) override;

    virtual int  GetPlayerId() override { return mPlayerId; }

    virtual void SetSceneId(int inSceneId) override { mSceneId = inSceneId; }
    virtual int GetSceneId() override { return mSceneId; }

    virtual void SetState(EPLAYER_STATE inPlayerState) override { mPlayerState = inPlayerState; }
    virtual EPLAYER_STATE GetState() override { return mPlayerState; }

    /* 当前该函数并没有啥用 */
    void SetGameThreadId(int inThreadId) { mGameThreadId = inThreadId; }
    int  GetGameThreadId() { return mGameThreadId; }

    /* 获得代理客户端对象 */
    static GClientProxy* GetClient(int inPlayerId);
private:
    /* 玩家当前所处的状态 */
    EPLAYER_STATE mPlayerState;
    /* 该客户端的唯一标识 */
    int mPlayerId;
    /* 消息处理线程ID */
    int mGameThreadId;
    /* 玩家所在场景ID */
    int mSceneId;

    static int mNewPlayerId;
    static std::unordered_map<int, GClientProxy*> mClientsMap;
};

#endif

