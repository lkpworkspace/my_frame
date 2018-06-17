#ifndef __IGMsgProcess_H__
#define __IGMsgProcess_H__

class MyList;

class IGMsgProcess
{
public:
    /* 缓存消息回调函数 */
    virtual void AppendMsg(GMsg* inMsg) = 0;

    /* server/client 消息处理回调函数 */
    virtual void ProcessMsgs() = 0;

    /* server/client 消息处理完后获得的回复信息 */
    virtual MyList* GetReplyMsg() = 0;
};

//typedef void (IGMsgProcess::*AppendMsgFunc_t)(GMsg* inMsg);
//typedef void (IGMsgProcess::*ProcessFunc_t)(MyList* inMsgs);
//typedef MyList* (IGMsgProcess::*ReplyFunc_t)();
//typedef void (*MsgProcessFunc_t)(MyList* inMsgs);

//#define RECVMSG_FUNC(func) (AppendMsgFunc_t)(&func)
//#define PROCESS_FUNC(func) (ProcessFunc_t)(&func)
//#define REPLY_FUNC(func) (ReplyFunc_t)(&func)


/**
  // 函数注册
    netmanager.RegMsgProcessFunc(EMT_RPC,
                                 &rpcmanager,
                                 PROCESS_FUNC(GRPCManager::ProcessFunc),
                                 REPLY_FUNC(GRPCManager::GetReplyMsg));
  // 函数使用
  ((func.classobj)->*(func.appendmsg))(msg);
  ((func.classobj)->*(func.func))(inMsg);
  ((func.classobj)->*(func.reply))();
*/
#endif
