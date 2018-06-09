#include <unordered_map>

#ifdef GSERVER
#include "GServer.h"
#else

#endif

GNetManager* GNetManager::sInstance = nullptr;

GNetManager* GNetManager::StaticInit()
{
    if(sInstance == nullptr)
        sInstance = new GNetManager();
    return sInstance;
}

void GNetManager::ProcessPacket(GMsg* inMsg)
{
    unsigned short msg_type;
    inMsg->Read(msg_type, 16);
    if(mMsgsProcessMap.find(msg_type) != mMsgsProcessMap.end())
    {
        ProcessMsg(msg_type, inMsg);
    }else
    {
        MyDebugPrint("MSGTYPE %d not Register\n", msg_type);
    }
}


bool GNetManager::RegMsgProcessFunc(unsigned short inMsgType,
                                    IGMsgProcess *inClass,
                                    ProcessFunc_t inProcessFunc,
                                    ReplyFunc_t inReplyFunc)
{
    if(mMsgsProcessMap.find(inMsgType) != mMsgsProcessMap.end())
    {
        MyDebugPrint("MSGTYPE %d Registered\n", inMsgType);
        return false;
    }
    GMsgFunc func;
    func.classobj = inClass;
    func.func = inProcessFunc;
    func.reply = inReplyFunc;
    mMsgsProcessMap[ inMsgType ] = func;
    return true;
}

/* call by ProcessPacket */
//TODO(lkp) test 添加回复函数的调用
void GNetManager::ProcessMsg(unsigned short inMsgType, GMsg *inMsg)
{
    if(mMsgsProcessMap.find(inMsgType) != mMsgsProcessMap.end())
    {
        GMsgFunc func = mMsgsProcessMap[inMsgType];
        ((func.classobj)->*(func.func))(inMsg);
        MyList* re_list = ((func.classobj)->*(func.reply))();
        mReplyMsgsList.Append(re_list);
    }else
    {
        MyDebugPrint("Has No MsgType %d\n", inMsgType);
    }
}

/* call by MyTask child class */
void GNetManager::SendOutgoingMsgs(void *data)
{
#ifdef GSERVER
    MyTask* task = (MyTask*)data;
    int send_identify = GetTaskIdentify("GReply");
    MyNode* begin = nullptr;
    MyNode* end = nullptr;
    MyNode* temp = nullptr;
    GReplyMsg* reply_msg = nullptr;

    begin = mReplyMsgsList.Begin();
    end = mReplyMsgsList.End();

    for(;begin != end;)
    {
        temp = begin->next;
        mReplyMsgsList.Del(begin);
        reply_msg = (GReplyMsg*)begin;
        reply_msg->SetSendIdentify(send_identify);

        task->AddSendEv(reply_msg);
        begin = temp;
    }
#else
    MyEasyTcpClient* client = (MyEasyTcpClient*)data;
    MyNode* begin = nullptr;
    MyNode* end = nullptr;
    MyNode* temp = nullptr;
    GReplyMsg* reply_msg = nullptr;

    begin = mReplyMsgsList->Begin();
    end = mReplyMsgsList->End();

    for(;begin != end;)
    {
        temp = begin->next;
        reply_msg = (GReplyMsg*)begin;
        mReplyMsgsList->Del(begin);
        client->EasyWrite(reply_msg->GetBufferPtr(), reply_msg->GetByteLength());
        begin = temp;
    }
#endif
}
