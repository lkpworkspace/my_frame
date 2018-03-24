#include "Server/MyServerShared.h"

MyNetworkManagerServer* MyNetworkManagerServer::sInstance = nullptr;

MyNetworkManagerServer::MyNetworkManagerServer()
    :mNewPlayerId(0),
      mNewNetworkId(0)
{
}

bool MyNetworkManagerServer::StaticInit()
{
    if(sInstance == nullptr)
        sInstance = new MyNetworkManagerServer();
    return true;
}


void	MyNetworkManagerServer::ProcessPacket( MyInputStream* inGameMsg)
{
    MyGameMsg* msg = (MyGameMsg*)inGameMsg;
    MyClientProxy* client = (MyClientProxy*)msg->GetClientProxy();
    MyDebugPrint("0x%08X\n",kHelloCC);
    //TODO(lkp): server process ...
    uint32_t	packetType;
    msg->Read( packetType );
    switch( packetType )
    {
    case kHelloCC:
        HandleHelloPack( msg, client);
        break;
    case kInputCC:
//        HandleInputPacket( msg, client );
        break;
    default:
        MyDebugPrint( "Unknown packet type received from %s", client->GetIp().c_str());
        break;
    }
    //MyGameServer::sInstance->mMsgPool.Free(msg);
#if 0 // test
    const char* buf = msg->GetBufferPtr();
    const int len = msg->GetByteLength();
    for(int i = 0; i < len; ++i)
    {
        printf("0x%X\t",buf[i]);
    }
    MyDebugPrint("\n");
#endif
}

void MyNetworkManagerServer::HandleHelloPack(MyGameMsg* inGameMsg, MyClientProxy* inClient)
{
    if(inClient->IsLogin())
        SendWelcomePack(inClient);
    else
    {
#if 0
        // read the name
        std::string name;
        inGameMsg->Read( name );
        inClient->SetName(name);
        // assign player id
        inClient->SetPlayerId(mNewPlayerId++);

        mPlayerIdToClientMap[ inClient->GetPlayerId() ] = inClient;

        //tell the server about this client, spawn a cat, etc...
        //if we had a generic message system, this would be a good use for it...
        //instead we'll just tell the server directly
//        static_cast< Server* > ( Engine::sInstance.get() )->HandleNewClient( newClientProxy );
        MyGameServer::sInstance->SpawnPlaneForPlayer( inClient->GetPlayerId() );

        //and welcome the client...
        SendWelcomePack( inClient );

        //and now init the replication manager with everything we know about!
        for( const auto& pair: mNetworkIdToGameObjectMap )
        {
//            inClient->GetReplicationManagerServer().ReplicateCreate( pair.first, pair.second->GetAllStateMask() );
        }
#endif
        MyDebugPrint("get hello msg\n");
        inClient->SetLogin(true);
    }
}

void MyNetworkManagerServer::HandleInputPack(MyGameMsg* inGameMsg, MyClientProxy* inClient)
{

}

void MyNetworkManagerServer::SendWelcomePack(MyClientProxy* inClient)
{
    MyOutputStream welcomePacket;

    welcomePacket.Write( kWelcomeCC );
    welcomePacket.Write( inClient->GetPlayerId() );

    MyDebugPrint( "Server Welcoming, new client '%s' as player %d", inClient->GetIp().c_str(), inClient->GetPlayerId() );

    inClient->EasyWrite( welcomePacket.GetBufferPtr(), welcomePacket.GetByteLength() );
}

void MyNetworkManagerServer::SendOutgoingPackets()
{

}
