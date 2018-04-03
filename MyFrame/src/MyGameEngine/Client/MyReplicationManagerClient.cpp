#include "MyGameClientShared.h"

void MyReplicationManagerClient::Read( MyInputStream& inInputStream )
{
    while( inInputStream.GetRemainingBitCount() >= 32 )
    {
        //read the network id...
        int networkId; inInputStream.Read( networkId );

        //only need 2 bits for action...
        uint8_t action; inInputStream.Read( action, 2 );

        switch( action )
        {
        case RA_Create:
            ReadAndDoCreateAction( inInputStream, networkId );
            break;
        case RA_Update:
            ReadAndDoUpdateAction( inInputStream, networkId );
            break;
        case RA_Destroy:
            ReadAndDoDestroyAction( inInputStream, networkId );
            break;
        }

    }

}

void MyReplicationManagerClient::ReadAndDoCreateAction( MyInputStream& inInputStream, int inNetworkId )
{
    //need 4 cc
    uint32_t fourCCName;
    inInputStream.Read( fourCCName );

    //we might already have this object- could happen if our ack of the create got dropped so server resends create request
    //( even though we might have created )
    MyGameObj* gameObject = MyNetworkManagerClient::sInstance->GetGameObject( inNetworkId );
    if( !gameObject )
    {
        //create the object and map it...
        gameObject = MyWorld::sInstance->CreateGameObject( fourCCName );
        gameObject->SetNetworkId( inNetworkId );
        MyNetworkManagerClient::sInstance->AddToNetworkIdToGameObjectMap( gameObject );
        printf("create gameobj 0x%08X, networkid %d\n", fourCCName, inNetworkId);
        //it had really be the rigth type...
        assert( gameObject->GetClassId() == fourCCName );
    }

    //and read state
    gameObject->Read( inInputStream );
}

void MyReplicationManagerClient::ReadAndDoUpdateAction( MyInputStream& inInputStream, int inNetworkId )
{
    //need object
    MyGameObj* gameObject = MyNetworkManagerClient::sInstance->GetGameObject( inNetworkId );

    //gameObject MUST be found, because create was ack'd if we're getting an update...
    //and read state
    gameObject->Read( inInputStream );
}

void MyReplicationManagerClient::ReadAndDoDestroyAction( MyInputStream& inInputStream, int inNetworkId )
{
    //if something was destroyed before the create went through, we'll never get it
    //but we might get the destroy request, so be tolerant of being asked to destroy something that wasn't created
    MyGameObj* gameObject = MyNetworkManagerClient::sInstance->GetGameObject( inNetworkId );
    if( gameObject )
    {
        gameObject->SetDoesWantToDie( true );
        //MyNetworkManagerClient::sInstance->RemoveFromNetworkIdToGameObjectMap( gameObject );
    }
}
