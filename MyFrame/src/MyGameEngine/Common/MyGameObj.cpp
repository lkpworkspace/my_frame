#include "MyGameEngineShared.h"

MyGameObj::MyGameObj() :
    mIndexInWorld( -1 ),
    mCollisionRadius( 0.5f ),
    mDoesWantToDie( false ),
    mRotation( 0.f ),
    mNetworkId( 0 ),
    mColor( MyColors::White ),
    mScale( 1.0f )
{
}

void MyGameObj::Update()
{
    //object don't do anything by default...
}


MyVec3 MyGameObj::GetForwardVector()	const
{
    //should we cache this when you turn?
    return MyVec3( sinf( mRotation ), -cosf( mRotation ), 0.f );
}

void MyGameObj::SetNetworkId( int inNetworkId )
{
    //this doesn't put you in the map or remove you from it
    mNetworkId = inNetworkId;

}

void MyGameObj::SetRotation( float inRotation )
{
    //should we normalize using fmodf?
    mRotation = inRotation;
}
