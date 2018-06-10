#ifndef __IGClientProxy_H__
#define __IGClientProxy_H__

class IGClientProxy
{
public:
    enum EPLAYER_STATE{
        /* 客户端正常退出 */
        EPS_QUIT,
        /* 客户端异常断开连接 */
        EPS_DISCONNECT,
        /* 客户端已经连接，但是没有进行登录验证 */
        EPS_CONNECT,
        /* 客户端已经登录 */
        EPS_LOGIN,
        /* 玩家正在进行排队匹配，等待开始游戏 */
        EPS_MATCHING,
        /* 玩家游戏中 */
        EPS_FINGHTING,
        /* 保留 */
        EPS_RESERVED,
    };

    IGClientProxy(){}
    virtual ~IGClientProxy(){}

    /* 设置/获得 玩家ID */
    virtual void SetPlayerId( int inPlayerId) = 0;
    virtual int  GetPlayerId() = 0;

    /* 设置/获得 客户端状态 */
    virtual void SetState(EPLAYER_STATE inPlayerState) = 0;
    virtual EPLAYER_STATE GetState() = 0;

    /* 设置/获得 玩家 */

};

#endif
