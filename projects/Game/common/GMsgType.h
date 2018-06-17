#ifndef __GMsgType_H__
#define __GMsgType_H__

/**
 * @brief The EMSG_TYPE enum
 *      客户端发送给服务器的消息类型
 *      或者
 *      服务器发送给客户端的消息类型
 */
enum EMSG_TYPE : unsigned short
{
    /* 登录请求/回复 */
    EMT_LOGIN,
    /* 匹配请求/回复 */
    EMT_MATCHING,
    /* 场景 */
    EMT_SCENE,
    /* 远程过程调用 */
    EMT_RPC,
};


/**
 * @brief The EMSG_TO enum
 *      回复消息头
 */
enum EMSG_TO : unsigned short
{
    /* 发送给客户端的消息 */
    EMTO_CLIENT,
    /* 发送给其他管理器的消息 */
    EMTO_MANAGER,
};

/**
 * @brief The EMSG_SCENE enum
 *      发送给 GSceneManager 消息的消息类型
 */
enum EMSG_SCENE : unsigned short
{
    EMS_CREATE,
    EMS_UPDATE,
    EMS_DESTORY
};


/**
  消息协议概述：
    客户端到服务器：
        EMSG_TYPE + message
            SetClientProxy
    服务器 --> 客户端或者其他管理器
        message
            SetMsgTo(EMSG_TYPE)
*/


#endif
