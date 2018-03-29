#include "Server/MyServerShared.h"

void MyPlaneServer::Update()
{
    MyPlane::Update();

    MyNetworkManagerServer::sInstance->SetStateDirty( GetNetworkId(), ECRS_Pose );
//    mLocation.mX = 1;
//    mLocation.mY = 2;
//    mLocation.mZ = 3;
#if 0
    Vector3 oldLocation = GetLocation();
    Vector3 oldVelocity = GetVelocity();
    float oldRotation = GetRotation();

    MyClientProxy* client = MyNetworkManagerServer::sInstance->GetClientProxy( GetPlayerId() );
    if( client )
    {
        MoveList& moveList = client->GetUnprocessedMoveList();
        for( const Move& unprocessedMove : moveList )
        {
            const InputState& currentState = unprocessedMove.GetInputState();
            float deltaTime = unprocessedMove.GetDeltaTime();
            ProcessInput( deltaTime, currentState );
            SimulateMovement( deltaTime );
        }

        moveList.Clear();
    }

    HandleShooting();

    if( !RoboMath::Is2DVectorEqual( oldLocation, GetLocation() ) ||
            !RoboMath::Is2DVectorEqual( oldVelocity, GetVelocity() ) ||
            oldRotation != GetRotation() )
    {
        NetworkManagerServer::sInstance->SetStateDirty( GetNetworkId(), ECRS_Pose );
    }
#endif
}
