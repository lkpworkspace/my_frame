#include "MyGameEngineShared.h"

MyGameObjCtrl::MyGameObjCtrl() :
    MyGameObj(),
    mCoordinate( 0, 0, 0 ),
    mCtrlType(ECT_Normal2D),
    mMaxRotationSpeed( 5.f ),
    mMaxLinearSpeed( 0.1f ),
    /* x轴方向为正方向 */
    mForwardDir( 0, 1, 0 ),
    mVelocity( MyVec3::Zero ),
    mThrustDir( 0.f )
{
}

uint32_t MyGameObjCtrl::WriteRotation2D( MyOutputStream& inOutputStream, uint32_t& inDirtyState ) const
{
    uint32_t writtenState = 0;

    return writtenState;
}

uint32_t MyGameObjCtrl::WriteNormal2D( MyOutputStream& inOutputStream, uint32_t& inDirtyState ) const
{
    uint32_t writtenState = 0;

    if( inDirtyState & ERS_LocalPos )
    {
        inOutputStream.Write( (bool)true );

        MyVec3 location = GetLocation();
        inOutputStream.Write( location.mX );
        inOutputStream.Write( location.mY );
        inOutputStream.Write( location.mZ );
        writtenState |= ERS_LocalPos;
    }
    else
    {
        inOutputStream.Write( (bool)false );
    }

    return writtenState;
}

uint32_t MyGameObjCtrl::Write( MyOutputStream& inOutputStream, uint32_t inDirtyState ) const
{
    uint32_t writtenState = 0;

    switch(mCtrlType)
    {
    case ECT_Normal2D:
        writtenState = WriteNormal2D(inOutputStream, inDirtyState);
        break;
    case ECT_Rotation2D:
        writtenState = WriteRotation2D(inOutputStream, inDirtyState);
        break;
    }

    return writtenState;
}

void MyGameObjCtrl::ReadRotation2D( MyInputStream& inInputStream )
{}

void MyGameObjCtrl::ReadNormal2D( MyInputStream& inInputStream )
{
    bool stateBit;
    uint32_t readState = 0;
    MyVec3 replicatedLocation;

    inInputStream.Read( stateBit );
    if( stateBit )
    {
        inInputStream.Read( replicatedLocation.mX );
        inInputStream.Read( replicatedLocation.mY );
        inInputStream.Read( replicatedLocation.mZ );
        SetLocation( replicatedLocation );

        readState |= ERS_LocalPos;
    }
}

void MyGameObjCtrl::Read( MyInputStream& inInputStream )
{
    switch(mCtrlType)
    {
    case ECT_Normal2D:
        ReadNormal2D(inInputStream);
        break;
    case ECT_Rotation2D:
        break;
    }
}

void MyGameObjCtrl::ProcessInput( float inDeltaTime, const MyInputState& inInputState )
{
    switch(mCtrlType)
    {
    case ECT_Normal2D:
    {
        float x,y;
        x = GetLocation().mX + inInputState.GetDesiredHorizontalDelta() * mMaxLinearSpeed * inDeltaTime;
        y = GetLocation().mY + inInputState.GetDesiredVerticalDelta() * mMaxLinearSpeed * inDeltaTime;

        SetLocation(MyVec3(x,y,GetLocation().mZ));
    }
        break;
    case ECT_Rotation2D:
        //turning...
        float newRotation = GetRotation() + inInputState.GetDesiredHorizontalDelta() * mMaxRotationSpeed * MyTiming::sInstance.GetDeltaTime();
        SetRotation( newRotation );

        //moving...
        float inputForwardDelta = inInputState.GetDesiredVerticalDelta();
        mThrustDir = inputForwardDelta;
        break;
    }
}

void MyGameObjCtrl::AdjustVelocityByThrust( float inDeltaTime )
{
    //just set the velocity based on the thrust direction -- no thrust will lead to 0 velocity
    //simulating acceleration makes the client prediction a bit more complex
    MyVec3 forwardVector = GetForwardVector();
    mVelocity = forwardVector * ( mThrustDir * inDeltaTime * mMaxLinearSpeed );
}

void MyGameObjCtrl::SimulateMovement( float inDeltaTime )
{
    switch(mCtrlType)
    {
    case ECT_Normal2D:
    {
    }
        break;
    case ECT_Rotation2D:
        AdjustVelocityByThrust( inDeltaTime );
        SetLocation( GetLocation() + mVelocity * inDeltaTime );
        break;
    }
}
