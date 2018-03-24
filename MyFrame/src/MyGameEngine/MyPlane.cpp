#include "MyGameEngine/MyGameEngineShared.h"

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
