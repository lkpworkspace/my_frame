#ifndef __MyGameObjCtrl_test_H__
#define __MyGameObjCtrl_test_H__

#include "MyServerShared.h"

class MyGameObjCtrl_test : public MyGameObjCtrl
{
public:
    CLASS_IDENTIFICATION( 'TEST', MyGameObjCtrl_test )
    static MyGameObj* StaticCreate() { return MyNetworkManagerServer::sInstance->RegisterGameObject( new MyGameObjCtrl_test() ); }

    virtual void HandleDying() override
    {
        MyNetworkManagerServer::sInstance->UnregisterGameObject(this);
    }

    virtual void Update() override
    {

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

                MyDebugPrint("deltatime %f, x %f, y %f\n", deltaTime, GetLocation().mX, GetLocation().mY);
            }

            moveList.Clear();
        }

#if 0
        if( !MyMath::Is2DVectorEqual( oldLocation, GetLocation() ) ||
                !MyMath::Is2DVectorEqual( oldVelocity, GetVelocity() ) ||
                oldRotation != GetRotation() )
        {
            MyNetworkManagerServer::sInstance->SetStateDirty( GetNetworkId(), ERS_LocalPos );
        }
#else
        MyNetworkManagerServer::sInstance->SetStateDirty( GetNetworkId(), ERS_LocalPos );
#endif
    }

#if 0
    static void Test()
    {
        MyGameObjCtrl ctrl;
        MyOutputStream ostream;
        MyInputStream istream;
        MyInputState state;

        bool forward = true;
        ostream.Write(forward);
        ostream.Write(forward);
        ostream.Write(forward);
        ostream.Write(forward);
        ostream.Write(forward);
        istream.SetBuffer(ostream.GetBufferPtr(), ostream.GetByteLength() * 8);
        state.Read(istream);

        ctrl.ProcessInput(0.5,state);

        printf("%f, %f, %f\n",
               ctrl.GetLocation().mX,
               ctrl.GetLocation().mY,
               ctrl.GetLocation().mZ);
    }
#endif
};

#endif
