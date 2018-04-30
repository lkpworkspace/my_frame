#include "MyServerShared.h"

void MyPlaneServer::HandleDying()
{
    MyNetworkManagerServer::sInstance->UnregisterGameObject(this);
}



void MyPlaneServer::Update()
{
    MyPlane::Update();
#if 0
    static float g_max_x = 50;
    static float g_max_y = 50;
    float dt = MyTiming::sInstance.GetDeltaTime();
    if(mLocation.mX < 50.0f)
    {
        mLocation.mX += dt * 10;
        mLocation.mY += dt * 10;
        mLocation.mZ = 0.0f;
    }else
    {
        mLocation.mX = 0.0f;
        mLocation.mY = 0.0f;
        mLocation.mZ = 0.0f;
    }

    //MyDebugPrint("plane X %f, Y %f, delat time %f ms\n",mLocation.mX, mLocation.mY, dt);

    MyNetworkManagerServer::sInstance->SetStateDirty( GetNetworkId(), ECRS_Pose );
#else
    MyVec3 oldLocation = GetLocation();
    MyVec3 oldVelocity = GetVelocity();
    float oldRotation = GetRotation();

    MyClientProxy* client = MyNetworkManagerServer::sInstance->GetClientProxy( GetPlayerId() );
    if( client )
    {
        MyMoveList& moveList = client->GetUnprocessedMoveList();
        for( const MyMove& unprocessedMove : moveList )
        {
            const MyInputState& currentState = unprocessedMove.GetInputState();
            float deltaTime = unprocessedMove.GetDeltaTime();
            ProcessInput( deltaTime, currentState );
            SimulateMovement( deltaTime );
            break;
        }

        moveList.Clear();
    }

    HandleShooting();

    if( !MyMath::Is2DVectorEqual( oldLocation, GetLocation() ) ||
            !MyMath::Is2DVectorEqual( oldVelocity, GetVelocity() ) ||
            oldRotation != GetRotation() )
    {
        MyNetworkManagerServer::sInstance->SetStateDirty( GetNetworkId(), ECRS_Pose );
    }
#endif
}

void MyPlaneServer::HandleShooting()
{
#if 0
    float time = MyTiming::sInstance.GetFrameStartTime();
    if( mIsShooting && Timing::sInstance.GetFrameStartTime() > mTimeOfNextShot )
    {
        //not exact, but okay
        mTimeOfNextShot = time + mTimeBetweenShots;

        //fire!
        YarnPtr yarn = std::static_pointer_cast< Yarn >( GameObjectRegistry::sInstance->CreateGameObject( 'YARN' ) );
        yarn->InitFromShooter( this );
    }
#endif
}
