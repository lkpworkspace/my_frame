#include "MyGameClientShared.h"

MyGameClient* MyGameClient::sInstance = nullptr;

bool MyGameClient::StaticInit(const std::string &inName, MyAddrInfo &info)
{
    // Create the Client pointer first because it initializes SDL
    sInstance = new MyGameClient();

    MyNetworkManagerClient::StaticInit(inName,info);
    return true;
}

MyGameClient::MyGameClient()
{
    MyWorld::StaticInit();
}

void MyGameClient::CollectEvent(MyList* inEvs)
{
    /// collect input event
    MyNetworkManagerClient::sInstance->HandleEvent(inEvs);
}

void MyGameClient::Update()
{
    MyInputStream inputStream;

    /// process incoming packets
    MyNetworkManagerClient::sInstance->GetClient().EasyRecv();
    while(true)
    {
        if(MyNetworkManagerClient::sInstance->GetClient().GetFrame(inputStream) > 0)
            MyNetworkManagerClient::sInstance->ProcessPacket(&inputStream);
        else
            break;
    }
    /// send outgoing packets
    MyNetworkManagerClient::sInstance->SendOutgoingPackets();
}
