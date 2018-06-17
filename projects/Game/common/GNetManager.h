#ifndef __GNetManager_H__
#define __GNetManager_H__

#include "IGMsgProcess.h"

/**
 * @brief The GNetManager class
 *      客户端/服务器 公用类
 *      需要添加一个各个管理器相互交互的方式
 */
class GNetManager
{
public:
    static GNetManager* sInstance;
    static GNetManager* StaticInit();

    /* 处理从客户端/服务器收到的消息 */
    void ProcessPacket(MyList* inMsgs);

    /* 注册处理消息的管理类 */
    bool RegMsgProcesssManager(unsigned short inMsgType,
                               IGMsgProcess* inMsgManager);

    /* 发送回复客户端/服务器的消息 */
    // server: data --> task 指针
    // client: data --> MyTcpClient 指针
    void SendOutgoingMsgs(void* data);

protected:
    /* 解析回复消息类型头 */
    void ParseMsgHeadType(MyList* re);
    /* 获得管理器 */
    IGMsgProcess* GetManager(unsigned short inManagerId);

private:
    /* 注册的消息处理对象 */
    std::unordered_map<unsigned short,IGMsgProcess*> mMsgsProcessMap;
    /* 缓存所有manager回复到客户端的消息列表 */
    MyList mReplyMsgsList;
    /* 缓存所有manager回复到manager的消息列表 */
    MyList mReplyToManagerList;
};

#endif




































///////////////////////////////////////////////////////////////////
//    class GMsgFunc
//    {
//    public:
//        IGMsgProcess* classobj;
//        AppendMsgFunc_t appendmsg;
//        ProcessFunc_t func;
//        ReplyFunc_t reply;
//    };

/* 注册消息处理类，以及处理方法(不使用) */
//    bool RegMsgProcessFunc(unsigned short inMsgType,
//                           IGMsgProcess* inClass,
//                           AppendMsgFunc_t inAppendMsgFunc,
//                           ProcessFunc_t inProcessFunc,
//                           ReplyFunc_t inReplyFunc);

/* call by ProcessPacket */
//    void ProcessMsg(unsigned short inMsgType, GMsg* inMsg);

/* 获得管理器的回调函数 */
//    GMsgFunc* GetManagerCall(unsigned short inManagerId);

/* 注册的消息回调函数 */
//    std::unordered_map<unsigned short,GMsgFunc> mMsgsProcessMap;
