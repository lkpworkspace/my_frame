#ifndef __GClientProxy_H__
#define __GClientProxy_H__

#include "IGClientProxy.h"

class GClientProxy : public myframe::MyEasyTcpSocket, public IGClientProxy
{
public:
    GClientProxy(int fd, sockaddr_in addr)
        :MyEasyTcpSocket(fd, addr),
          mPlayerState(EPS_CONNECT),
          mPlayerId(-1),
          mGameThreadId(-1)
    {

    }

    virtual ~GClientProxy(){}

    /* override MyEasyTcpSocket method */
    virtual int Frame(const char* buf, int len) override;

    virtual void SetPlayerId( int inPlayerId) override { mPlayerId = inPlayerId; }
    virtual int  GetPlayerId() override { return mPlayerId; }

    virtual void SetState(EPLAYER_STATE inPlayerState) override { mPlayerState = inPlayerState; }
    virtual EPLAYER_STATE GetState() override { return mPlayerState; }

    void SetGameThreadId(int inThreadId) { mGameThreadId = inThreadId; }
    int  GetGameThreadId() { return mGameThreadId; }

private:
    /* 玩家当前所处的状态 */
    EPLAYER_STATE mPlayerState;
    /* 该客户端的唯一标识 */
    int mPlayerId;
    /* 消息处理线程ID */
    int mGameThreadId;
};

#endif

