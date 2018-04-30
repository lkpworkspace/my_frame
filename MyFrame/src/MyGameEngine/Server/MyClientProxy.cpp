#include "MyServerShared.h"

void PrintBuf(const char* inBuf, int inLen)
{
    for(int i = 0; i < inLen; ++i)
    {
        printf("0x%02X\t",(char)inBuf[i]);
    }
    printf("\n");
}

/**
 * Frame()
 * @buf:   传入的消息的buf
 * @len:   消息的长度(如果为0,表示客户端退出)
 * Return: 返回true表示继续监听该事件，false表示不再监听此事件
 */
int MyClientProxy::Frame(const char* buf, int len)
{
    MyGameMsg* msg = (MyGameMsg*)MyGameServer::sInstance->mMsgPool.Get("MSG_GameMsg");

    if(len == 0)
    {
        MyDebugPrint("player %d client quit\n", mPlayerId);
        mIsClientQuit = true;
        msg->SetBuffer(buf,len * 8);
        msg->SetClientProxy(this);
        msg->SetSendIdentify(MyGameServer::sInstance->mGameEngineTaskId);
        AddSendEv(msg);
        return false;
    }
#if 1
    //MyDebugPrint("player %d client get msg: %s, pointer %p\n",mPlayerId, buf,msg);
    PrintBuf(buf,len);
#endif
    msg->SetBuffer(buf,len * 8);
    msg->SetClientProxy(this);
    msg->SetSendIdentify(MyGameServer::sInstance->mGameEngineTaskId);

    AddSendEv(msg);
    return true;
}
