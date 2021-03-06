#ifndef MyPlane_H
#define MyPlane_H

class MyPlane : public MyGameObj
{
public:
    CLASS_IDENTIFICATION( 'PLAN', MyPlane )
    enum ECatReplicationState
    {
        ECRS_Pose = 1 << 0,
        ECRS_Color = 1 << 1,
        ECRS_PlayerId = 1 << 2,
        ECRS_Health = 1 << 3,

        ECRS_AllState = ECRS_Pose | ECRS_Color | ECRS_PlayerId | ECRS_Health
    };
    virtual uint32_t GetAllStateMask() const override { return ECRS_AllState; }

    void ProcessInput( float inDeltaTime, const MyInputState& inInputState );
    void SimulateMovement( float inDeltaTime );

    void		SetPlayerId( uint32_t inPlayerId )			{ mPlayerId = inPlayerId; }
    uint32_t	GetPlayerId()						const 	{ return mPlayerId; }

//    void ProcessCollisions();
//	void ProcessCollisionsWithScreenWalls();

    void SetVelocity( const MyVec3& inVelocity )	{ mVelocity = inVelocity; }
    const MyVec3&	GetVelocity() const { return mVelocity; }

    virtual uint32_t	Write( MyOutputStream& inOutputStream, uint32_t inDirtyState ) const;
protected:
    MyPlane();

    uint32_t mPlayerId;

    MyVec3 mVelocity;

    float mThrustDir;

    int mHealth;
private:
    void AdjustVelocityByThrust( float inDeltaTime );

    float mMaxLinearSpeed;
    float mMaxRotationSpeed;

//    float mWallRestitution;
//    float mCatRestitution;

    bool mIsShooting;
};

#endif

