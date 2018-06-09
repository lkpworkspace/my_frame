#include "MyGameEngineShared.h"

MyGameObj::MyGameObj() :
    mIndexInWorld( -1 ),
    mNetworkId( 0 ),
    mCollisionRadius( 0.1f ),
    mDoesWantToDie( false ),
    mRotation( 0.f ),
    mColor( MyColors::Blue ),
    mScale( 1.0f )
{
}

void MyGameObj::Update()
{}


MyVec3 MyGameObj::GetForwardVector()	const
{
    return MyVec3( sinf( mRotation ), -cosf( mRotation ), 0.f );
}

void MyGameObj::SetNetworkId( int inNetworkId )
{
    mNetworkId = inNetworkId;
}

void MyGameObj::SetRotation( float inRotation )
{
    mRotation = inRotation;
}
