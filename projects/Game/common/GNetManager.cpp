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

void GNetManager::ProcessPacket(MyList *inMsgs)
{
    // 根据消息类型进行分类
    unsigned short manager_type;
    GMsg* temp = nullptr;
    GMsg* msg = (GMsg*)inMsgs->Begin();
    for(;msg != inMsgs->End();)
    {
        temp = (GMsg*)msg->next;
        inMsgs->Del(msg);
        msg->Read(manager_type, 16);

        if(mMsgsProcessMap.find(manager_type) != mMsgsProcessMap.end())
        {
            IGMsgProcess* manager = mMsgsProcessMap[manager_type];
            manager->AppendMsg(msg);
        }else
        {
            MyDebugPrint("MSGTYPE %d not Register\n", manager_type);
        }
        msg = temp;
    }

    auto iter = mMsgsProcessMap.begin();
    for(;iter != mMsgsProcessMap.end(); iter++)
    {
        // 消息处理
        IGMsgProcess*manager = ((IGMsgProcess*)(iter->second));
        manager->ProcessMsgs();
        // 缓存回复消息,如果是发送给其他管理器的消息则重新调用管理器处理函数
        MyList* reply_msg = manager->GetReplyMsg();
        //TODO(lkp) 分析消息类型，是发送给客户端的，还是发送给其他Manager
        //...如果是其他Manager的就调用其他Manager的处理函数
        //...否则将消息加入到回复队列中即可
        //...是否要考虑多线程互斥的问题？？？
        ParseMsgHeadType(reply_msg);
        mReplyMsgsList.Append(reply_msg);
    }
    return;
}

bool GNetManager::RegMsgProcesssManager(unsigned short inMsgType,
                                    IGMsgProcess *inMsgManager)
{
    if(mMsgsProcessMap.find(inMsgType) != mMsgsProcessMap.end())
    {
        MyDebugPrint("MSGTYPE %d Registered\n", inMsgType);
        return false;
    }
    mMsgsProcessMap[ inMsgType ] = inMsgManager;
    return true;
}



void GNetManager::ParseMsgHeadType(MyList* re)
{
    MyNode* begin = nullptr;
    MyNode* end = nullptr;
    MyNode* temp = nullptr;
    GReplyMsg* reply_msg = nullptr;
    GMsg* msg = nullptr;
    IGMsgProcess* manager = nullptr;

    begin = re->Begin();
    end = re->End();

    for(;begin != end;)
    {
        temp = begin->next;

        reply_msg = (GReplyMsg*)begin;
        switch (reply_msg->GetMsgTo()) {
        case EMTO_MANAGER:
            reply_msg = (GReplyMsg*)begin;
            manager = GetManager(reply_msg->GetMsgToId());
            if(manager)
            {
                msg = GetGMsg();
                msg->SetBuffer(reply_msg->GetBufferPtr(), reply_msg->GetByteLength()*8);
                re->Del(begin);
                FreeGReplyMsg(reply_msg);
                manager->AppendMsg(msg);
                manager->ProcessMsgs();
            }
            break;
        case EMTO_CLIENT:
        default:
            break;
        }
        begin = temp;
    }
}



IGMsgProcess* GNetManager::GetManager(unsigned short inManagerId)
{
    if(mMsgsProcessMap.find(inManagerId) == mMsgsProcessMap.end())
    {
        MyDebugPrint("Error Manager ID %d\n",inManagerId);
        return nullptr;
    }
    return mMsgsProcessMap[inManagerId];
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

































///////////////////////////////////////////////////////////////////////////////////
//GNetManager::GMsgFunc* GNetManager::GetManagerCall(unsigned short inManagerId)
//{
//    if(mMsgsProcessMap.find(inManagerId) == mMsgsProcessMap.end())
//    {
//        MyDebugPrint("Error Manager ID %d\n",inManagerId);
//        return nullptr;
//    }
//    return &(mMsgsProcessMap[inManagerId]);
//}


//bool GNetManager::RegMsgProcessFunc(unsigned short inMsgType,
//                                    IGMsgProcess *inClass, AppendMsgFunc_t inAppendMsgFunc,
//                                    ProcessFunc_t inProcessFunc,
//                                    ReplyFunc_t inReplyFunc)
//{
//    if(mMsgsProcessMap.find(inMsgType) != mMsgsProcessMap.end())
//    {
//        MyDebugPrint("MSGTYPE %d Registered\n", inMsgType);
//        return false;
//    }
//    GMsgFunc func;
//    func.classobj = inClass;
//    func.appendmsg = inAppendMsgFunc;
//    func.func = inProcessFunc;
//    func.reply = inReplyFunc;
//    mMsgsProcessMap[ inMsgType ] = func;
//    return true;
//}

/* call by ProcessPacket */
//void GNetManager::ProcessMsg(unsigned short inMsgType, GMsg *inMsg)
//{
//    if(mMsgsProcessMap.find(inMsgType) != mMsgsProcessMap.end())
//    {
//        GMsgFunc func = mMsgsProcessMap[inMsgType];
//        ((func.classobj)->*(func.func))(inMsg);

//        MyList* re_list = ((func.classobj)->*(func.reply))();
//        //TODO(lkp) 分析消息类型，是发送给客户端的，还是发送给其他Manager
//        //...如果是其他Manager的就调用其他Manager的处理函数
//        //...否则将消息加入到回复队列中即可
//        //...是否要考虑多线程互斥的问题？？？
//        ParseMsgHeadType(re_list);
//        mReplyMsgsList.Append(re_list);
//    }else
//    {
//        MyDebugPrint("Has No MsgType %d\n", inMsgType);
//    }
//}


//    inMsg->Read(manager_type, 16);
//    if(mMsgsProcessMap.find(manager_type) != mMsgsProcessMap.end())
//    {
//        ProcessMsg(manager_type, inMsg);
//    }else
//    {
//        MyDebugPrint("MSGTYPE %d not Register\n", manager_type);
//    }
