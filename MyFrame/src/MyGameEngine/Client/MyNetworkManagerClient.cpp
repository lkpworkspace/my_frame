#include "Client/MyGameClientShared.h"

MyNetworkManagerClient* MyNetworkManagerClient::sInstance = nullptr;

void MyNetworkManagerClient::StaticInit(const std::string& inName , MyAddrInfo &inInfo)
{
    sInstance = new MyNetworkManagerClient();
    sInstance->mName = inName;
    sInstance->mSimpleTcpClient.SetAddrInfo(inInfo);
    sInstance->mSimpleTcpClient.Connect();
}

MyNetworkManagerClient::MyNetworkManagerClient()
    :mName(""),
      mState(NCS_SayingHello),
      mPlayerId(-1)
{

}

void MyNetworkManagerClient::HandleEvent(MyList* inEvs)
{

}

void MyNetworkManagerClient::ProcessPacket( MyInputStream* inInputStream)
{
    uint32_t	packetType;
    inInputStream->Read( packetType );
    switch( packetType )
    {
    case kWelcomeCC:
        HandleWelcomePacket( *inInputStream );
        break;
    case kStateCC:
        HandleStatePacket( *inInputStream );
        break;
    default:
        printf( "Unknown packet type received 0x%08X\n",packetType);
        break;
    }
}

void MyNetworkManagerClient::SendOutgoingPackets()
{
    switch( mState )
    {
    case NCS_SayingHello:
        UpdateSayingHello();
        break;
    case NCS_Welcomed:
        UpdateSendingInputPacket();
        break;
    }
}

void MyNetworkManagerClient::UpdateSayingHello()
{
#if 0
//    float time = Timing::sInstance.GetTimef();

//    if( time > mTimeOfLastHello + kTimeBetweenHellos )
//    {
//        SendHelloPacket();
//        mTimeOfLastHello = time;
//    }
#else
    SendHelloPacket();
    printf("send hello packet\n");
#endif
}

void MyNetworkManagerClient::SendHelloPacket()
{
    MyOutputStream helloPacket;

    helloPacket.Write( kHelloCC );
    helloPacket.Write( mName );

    mSimpleTcpClient.EasySend(helloPacket.GetBufferPtr(), helloPacket.GetByteLength());
}

void MyNetworkManagerClient::UpdateSendingInputPacket()
{
#if 0
    float time = Timing::sInstance.GetTimef();

    if( time > mTimeOfLastInputPacket + kTimeBetweenInputPackets )
    {
        SendInputPacket();
        mTimeOfLastInputPacket = time;
    }
#endif
    SendInputPacket();
    //printf("send Input packet(do nothing)\n");
}

void MyNetworkManagerClient::SendInputPacket()
{
#if 0
    //only send if there's any input to send!
    MoveList& moveList = InputManager::sInstance->GetMoveList();

    if( moveList.HasMoves() )
    {
        OutputMemoryBitStream inputPacket;
        inputPacket.Write( kInputCC );

        //we only want to send the last three moves
        int moveCount = moveList.GetMoveCount();
        int startIndex = moveCount > 3 ? moveCount - 3 - 1 : 0;
        inputPacket.Write( moveCount - startIndex, 2 );
        for( int i = startIndex; i < moveCount; ++i )
        {
            moveList[i].Write( inputPacket );
        }

        SendPacket( inputPacket, mServerAddress );
        moveList.Clear();
    }
#endif
}

void MyNetworkManagerClient::HandleWelcomePacket( MyInputStream& inInputStream )
{
    if( mState == NCS_SayingHello )
    {
        //if we got a player id, we've been welcomed!
        int playerId;
        inInputStream.Read( playerId );
        mPlayerId = playerId;
        mState = NCS_Welcomed;
    }
    printf("get welcome msg, playerId %d\n",mPlayerId);
}

void MyNetworkManagerClient::HandleStatePacket( MyInputStream& inInputStream )
{
    if( mState == NCS_Welcomed )
    {
        //ReadLastMoveProcessedOnServerTimestamp( inInputStream );

        //old
        //HandleGameObjectState( inPacketBuffer );
        //HandleScoreBoardState( inInputStream );

        //tell the replication manager to handle the rest...
        mReplicationManagerClient.Read( inInputStream );
        printf("get state packet\n");
    }
}

void MyNetworkManagerClient::HandleGameObjectState( MyInputStream& inInputStream )
{
#if 0
    //copy the mNetworkIdToGameObjectMap so that anything that doesn't get an updated can be destroyed...
    IntToGameObjectMap	objectsToDestroy = mNetworkIdToGameObjectMap;

    int stateCount;
    inInputStream.Read( stateCount );
    if( stateCount > 0 )
    {
        for( int stateIndex = 0; stateIndex < stateCount; ++stateIndex )
        {
            int networkId;
            uint32_t fourCC;

            inInputStream.Read( networkId );
            inInputStream.Read( fourCC );
            GameObjectPtr go;
            auto itGO = mNetworkIdToGameObjectMap.find( networkId );
            //didn't find it, better create it!
            if( itGO == mNetworkIdToGameObjectMap.end() )
            {
                go = GameObjectRegistry::sInstance->CreateGameObject( fourCC );
                go->SetNetworkId( networkId );
                AddToNetworkIdToGameObjectMap( go );
            }
            else
            {
                //found it
                go = itGO->second;
            }

            //now we can update into it
            go->Read( inInputStream );
            objectsToDestroy.erase( networkId );
        }
#endif
}
