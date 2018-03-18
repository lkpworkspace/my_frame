#include "Server/MyServerShared.h"


/**
 * Frame()
 * @buf:   传入的消息的buf
 * @len:   消息的长度(如果为0,表示客户端退出)
 * Return: 返回true表示继续监听该事件，false表示不再监听此事件
 */
int MyClientProxy::Frame(const char* buf, int len)
{
    MyGameMsg* msg = new MyGameMsg();

    if(len == 0)
    {
        MyDebugPrint("client quit\n");
        return false;
    }

    MyDebugPrint("Get Msg: %s, %p\n",buf,msg);
    msg->SetBuffer(buf,len * 8);
    msg->SetClientProxy(this);
    msg->SetSendIdentify(MyGameServer::sInstance->mGameEngineTaskId);

    AddSendEv(msg);
    return true;
}
