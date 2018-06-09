#ifndef __GNetManager_H__
#define __GNetManager_H__

#include "IGMsgProcess.h"

/**
 * @brief The GNetManager class
 *      客户端/服务器 公用类
 */
class GNetManager
{
    struct GMsgFunc
    {
        IGMsgProcess* classobj;
        ProcessFunc_t func;
        ReplyFunc_t reply;
    };
public:
    static GNetManager* sInstance;
    static GNetManager* StaticInit();

    /* 处理从客户端/服务器收到的消息 */
    void ProcessPacket(GMsg* inMsg);

    /* 注册消息处理类，以及处理方法 */
    bool RegMsgProcessFunc(unsigned short inMsgType,
                           IGMsgProcess* inClass,
                           ProcessFunc_t inProcessFunc,
                           ReplyFunc_t inReplyFunc);

    /* 发送回复客户端/服务器的消息 */
    // server: data --> task 指针
    // client: data --> MyTcpClient 指针
    void SendOutgoingMsgs(void* data);

protected:
    /* call by ProcessPacket */
    void ProcessMsg(unsigned short inMsgType, GMsg* inMsg);

private:
    /* 注册的消息回调函数 */
    std::unordered_map<unsigned short, GMsgFunc> mMsgsProcessMap;
    MyList mReplyMsgsList;
};

#endif
