#include <unordered_map>

#include "MyFrame.h"
#include "GUtils.h"
#include "GMsg.h"
#include "IGMsgProcess.h"
#include "GRPCManager.h"

void GRPCManager::ProcessMsgs()
{
    GMsg* temp = nullptr;
    GMsg* msg = (GMsg*)mRecvMsgs.Begin();
    for(;msg != mRecvMsgs.End();)
    {
        temp = (GMsg*)msg->next;
        mRecvMsgs.Del(msg);

        ProcessMsg(msg);

        FreeGMsg(msg);
        msg = temp;
    }
}

void GRPCManager::ProcessMsg(GMsg* inMsg)
{
    std::string func_name;
    GReplyMsg* reply_msg;

    inMsg->Read(func_name);
    if(mRPCFuncsMap.find(func_name) != mRPCFuncsMap.end())
    {
        reply_msg = GetGReplyMsg();
        mRPCFuncsMap[ func_name ](inMsg, reply_msg);
        reply_msg->SetClientProxy(inMsg->GetClientProxy());
        mReplicationMsg.AddTail(reply_msg);
    }else
    {
        MyDebugPrint("RPC %s Not Registed\n", func_name.c_str());
    }
}

bool GRPCManager::RegRPCFunc(std::string inFuncName, RPCFunc_t inFunc)
{
    if(inFuncName.empty() ||
       (mRPCFuncsMap.find(inFuncName) != mRPCFuncsMap.end()))
    {
        MyDebugPrint("RPC %s Has Registed\n", inFuncName.c_str());
        return false;
    }
    mRPCFuncsMap[inFuncName] = inFunc;
    return true;
}
