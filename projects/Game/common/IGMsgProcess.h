#ifndef __IGMsgProcess_H__
#define __IGMsgProcess_H__

class GMsg;
class MyList;

class IGMsgProcess
{
public:
    /* server/client 消息处理回调函数 */
    virtual void ProcessFunc(GMsg* inMsg) = 0;

    /* server/client 消息处理完后获得的回复信息 */
    virtual MyList* GetReplyMsg() = 0;
};

typedef void (IGMsgProcess::*ProcessFunc_t)(GMsg* msg);
typedef MyList* (IGMsgProcess::*ReplyFunc_t)();
typedef void (*MsgProcessFunc_t)(GMsg* msg);

#define PROCESS_FUNC(func) (ProcessFunc_t)(&func)
#define REPLY_FUNC(func) (ReplyFunc_t)(&func)

#endif
