#include <unordered_map>

#include "MyFrame.h"
#include "GUtils.h"
#include "GMsg.h"
#include "IGMsgProcess.h"
#include "GRPCManager.h"

void GRPCManager::ProcessFunc(GMsg* inMsg)
{
    std::string func_name;
    GMsg* msg;

    inMsg->Read(func_name);
    if(mRPCFuncsMap.find(func_name) != mRPCFuncsMap.end())
    {
        msg = GetGMsg();
        mRPCFuncsMap[ func_name ](inMsg, msg);
        msg->SetClientProxy(inMsg->GetClientProxy());
        mReplicationMsg.AddTail(msg);
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
