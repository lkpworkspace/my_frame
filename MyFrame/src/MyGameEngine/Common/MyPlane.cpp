#include "MyGameEngineShared.h"

MyPlane::MyPlane() :
    MyGameObj(),
    mMaxRotationSpeed( 5.f ),
    mMaxLinearSpeed( 50.f ),
    mVelocity( MyVec3::Zero ),
    mThrustDir( 0.f ),
    mPlayerId( -1 ),
    mIsShooting( false ),
    mHealth( 10 )
{

}

uint32_t MyPlane::Write( MyOutputStream& inOutputStream, uint32_t inDirtyState ) const
{
    uint32_t writtenState = 0;

    if( inDirtyState & ECRS_PlayerId )
    {
        inOutputStream.Write( (bool)true );
        inOutputStream.Write( GetPlayerId() );

        writtenState |= ECRS_PlayerId;
    }
    else
    {
        inOutputStream.Write( (bool)false );
    }


    if( inDirtyState & ECRS_Pose )
    {
        inOutputStream.Write( (bool)true );

        MyVec3 velocity = mVelocity;
        inOutputStream.Write( velocity.mX );
        inOutputStream.Write( velocity.mY );

        MyVec3 location = GetLocation();
        inOutputStream.Write( location.mX );
        inOutputStream.Write( location.mY );

        inOutputStream.Write( GetRotation() );

        writtenState |= ECRS_Pose;
    }
    else
    {
        inOutputStream.Write( (bool)false );
    }

    //always write mThrustDir- it's just two bits
    if( mThrustDir != 0.f )
    {
        inOutputStream.Write( true );
        inOutputStream.Write( mThrustDir > 0.f );
    }
    else
    {
        inOutputStream.Write( false );
    }

    if( inDirtyState & ECRS_Color )
    {
        inOutputStream.Write( (bool)true );
        inOutputStream.Write( GetColor() );

        writtenState |= ECRS_Color;
    }
    else
    {
        inOutputStream.Write( (bool)false );
    }

    if( inDirtyState & ECRS_Health )
    {
        inOutputStream.Write( (bool)true );
        inOutputStream.Write( mHealth, 4 );

        writtenState |= ECRS_Health;
    }
    else
    {
        inOutputStream.Write( (bool)false );
    }


    return writtenState;
}

void MyPlane::ProcessInput( float inDeltaTime, const MyInputState& inInputState )
{
    //process our input....

    //turning...
    float newRotation = GetRotation() + inInputState.GetDesiredHorizontalDelta() * mMaxRotationSpeed * MyTiming::sInstance.GetDeltaTime();
    SetRotation( newRotation );

    //moving...
    float inputForwardDelta = inInputState.GetDesiredVerticalDelta();
    mThrustDir = inputForwardDelta;


    mIsShooting = inInputState.IsShooting();
}

void MyPlane::AdjustVelocityByThrust( float inDeltaTime )
{
    //just set the velocity based on the thrust direction -- no thrust will lead to 0 velocity
    //simulating acceleration makes the client prediction a bit more complex
    MyVec3 forwardVector = GetForwardVector();
    mVelocity = forwardVector * ( mThrustDir * inDeltaTime * mMaxLinearSpeed );
}

void MyPlane::SimulateMovement( float inDeltaTime )
{
    //simulate us...
    AdjustVelocityByThrust( MyTiming::sInstance.GetDeltaTime() );

    SetLocation( GetLocation() + mVelocity * MyTiming::sInstance.GetDeltaTime() );

    //ProcessCollisions();
}
