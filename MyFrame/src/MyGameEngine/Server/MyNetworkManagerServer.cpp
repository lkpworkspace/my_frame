#include "Server/MyServerShared.h"

MyNetworkManagerServer* MyNetworkManagerServer::sInstance = nullptr;

void PrintInputStream(MyInputStream* inInputStream)
{
#if 1 // test
    const char* buf = inInputStream->GetBufferPtr();
    const int len = inInputStream->GetByteLength();
    for(int i = 0; i < len; ++i)
    {
        printf("0x%X\t",buf[i]);
    }
    printf("\n");
#endif
}

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

// TODO(lkp)...
void MyNetworkManagerServer::HandleClientDisconnected( MyClientProxy* inClientProxy )
{
    mPlayerIdToClientMap.erase( inClientProxy->GetPlayerId() );
    //static_cast< Server* > ( Engine::sInstance.get() )->HandleLostClient( inClientProxy );
}

void MyNetworkManagerServer::ProcessPacket( MyInputStream* inGameMsg)
{
    MyGameMsg* msg = (MyGameMsg*)inGameMsg;
    MyClientProxy* client = (MyClientProxy*)msg->GetClientProxy();
    // client quit
    if(client->IsQuit())
    {
        HandleClientDisconnected(client);
        return;
    }
    //TODO(lkp): server process ...
    uint32_t	packetType;
    msg->Read( packetType );
    MyDebugPrint("kHelloCC 0x%08X\n",kHelloCC);
    MyDebugPrint("kWelcomeCC 0x%08X\n",kWelcomeCC);
    MyDebugPrint("kInputCC 0x%08X\n",kInputCC);
    MyDebugPrint("kStateCC 0x%08X\n",kStateCC);
    MyDebugPrint("packetType 0x%08X\n",packetType);
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
    MyGameServer::sInstance->mMsgPool.Free(msg);
}

void MyNetworkManagerServer::HandleHelloPack(MyGameMsg* inGameMsg, MyClientProxy* inClient)
{
    if(inClient->IsLogin())
        SendWelcomePack(inClient);
    else
    {
#if 1
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
        HandleNewClient(inClient);

        //and welcome the client...
        SendWelcomePack( inClient );

        //and now init the replication manager with everything we know about!
        for( const auto& pair: mNetworkIdToGameObjectMap )
        {
            inClient->GetReplicationManagerServer().ReplicateCreate( pair.first, pair.second->GetAllStateMask() );
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

    MyDebugPrint( "Server Welcoming, new client '%s' as player %d\n", inClient->GetIp().c_str(), inClient->GetPlayerId() );

    inClient->EasyWrite( welcomePacket.GetBufferPtr(), welcomePacket.GetByteLength() );
}

void MyNetworkManagerServer::SendOutgoingPackets()
{
    //let's send a client a state packet whenever their move has come in...
    for( auto it = mPlayerIdToClientMap.begin(), end = mPlayerIdToClientMap.end(); it != end; ++it )
    {
        MyClientProxy* clientProxy = it->second;

        SendStatePacketToClient( clientProxy );
    }
}

void MyNetworkManagerServer::SendStatePacketToClient( MyClientProxy* inClientProxy )
{
    //build state packet
    MyOutputStream	statePacket;

    //it's state!
    statePacket.Write( kStateCC );

    //WriteLastMoveTimestampIfDirty( statePacket, inClientProxy );
    //AddScoreBoardStateToPacket( statePacket );

    inClientProxy->GetReplicationManagerServer().Write( statePacket );
    inClientProxy->EasyWrite(statePacket.GetBufferPtr(), statePacket.GetByteLength());
}


void MyNetworkManagerServer::HandleNewClient( MyClientProxy* inClientProxy )
{
    int playerId = inClientProxy->GetPlayerId();

    //ScoreBoardManager::sInstance->AddEntry( playerId, inClientProxy->GetName() );
    //SpawnCatForPlayer( playerId );
    MyPlane* plane = (MyPlane*)( MyWorld::sInstance->CreateGameObject( 'PLAN' ) );
    //plane->SetColor( ScoreBoardManager::sInstance->GetEntry( inPlayerId )->GetColor() );
    plane->SetPlayerId( playerId );
    //gotta pick a better spawn location than this...
    plane->SetLocation( MyVec3( 1.f - static_cast< float >( playerId ), 0.f, 0.f ) );
}

void MyNetworkManagerServer::HandleLostClient( MyClientProxy* inClientProxy )
{
#if 0
    int playerId = inClientProxy->GetPlayerId();

    //ScoreBoardManager::sInstance->RemoveEntry( playerId );
    MyPlane* plane = GetPlaneForPlayer( playerId );
    if( plane )
    {
        plane->SetDoesWantToDie( true );
    }
#endif
}

MyGameObj *MyNetworkManagerServer::RegisterGameObject( MyGameObj* inGameObject )
{
    //assign network id
    int newNetworkId = mNewNetworkId++;
    inGameObject->SetNetworkId( newNetworkId );

    //add mapping from network id to game object
    mNetworkIdToGameObjectMap[ newNetworkId ] = inGameObject;

    //tell all client proxies this is new...
    for( const auto& pair: mPlayerIdToClientMap )
    {
        pair.second->GetReplicationManagerServer().ReplicateCreate( newNetworkId, inGameObject->GetAllStateMask() );
    }

    return inGameObject;
}


void MyNetworkManagerServer::SetStateDirty( int inNetworkId, uint32_t inDirtyState )
{
    //tell everybody this is dirty
    for( const auto& pair: mPlayerIdToClientMap )
    {
        pair.second->GetReplicationManagerServer().SetStateDirty( inNetworkId, inDirtyState );
    }
}
