#ifndef MyGameObj_H
#define MyGameObj_H

#define CLASS_IDENTIFICATION( inCode, inClass ) \
enum { kClassId = inCode }; \
virtual uint32_t GetClassId() const { return kClassId; } \
static MyGameObj* CreateInstance() { return static_cast< MyGameObj* >( new inClass() ); } \


class MyGameObj
{
public:

    CLASS_IDENTIFICATION( 'GOBJ', MyGameObj )

    MyGameObj();
    virtual ~MyGameObj() {}

//    virtual	RoboCat*	GetAsCat()	{ return nullptr; }

    virtual uint32_t GetAllStateMask()	const { return 0; }

    //return whether to keep processing collision
//    virtual bool	HandleCollisionWithCat( RoboCat* inCat ) { ( void ) inCat; return true; }

    virtual void	Update();

    virtual void	HandleDying() {}

            void	SetIndexInWorld( int inIndex )						{ mIndexInWorld = inIndex; }
            int		GetIndexInWorld()				const				{ return mIndexInWorld; }

            void	SetRotation( float inRotation );
            float	GetRotation()					const				{ return mRotation; }

            void	SetScale( float inScale )							{ mScale = inScale; }
            float	GetScale()						const				{ return mScale; }


    const MyVec3&		GetLocation()				const				{ return mLocation; }
            void		SetLocation( const MyVec3& inLocation )		{ mLocation = inLocation; }

            float		GetCollisionRadius()		const				{ return mCollisionRadius; }
            void		SetCollisionRadius( float inRadius )			{ mCollisionRadius = inRadius; }

            MyVec3		GetForwardVector()			const;

            /* get/set clolor */
            void		SetColor( const MyVec3& inColor )					{ mColor = inColor; }
    const MyVec3&		GetColor()					const				{ return mColor; }
            /* 是否死亡 */
            bool		DoesWantToDie()				const				{ return mDoesWantToDie; }
            void		SetDoesWantToDie( bool inWants )				{ mDoesWantToDie = inWants; }

            /* get/set 网络标识 */
            int			GetNetworkId()				const				{ return mNetworkId; }
            void		SetNetworkId( int inNetworkId );

    virtual uint32_t	Write( MyOutputStream& inOutputStream, uint32_t inDirtyState ) const	{  ( void ) inOutputStream; ( void ) inDirtyState; return 0; }
    virtual void		Read( MyInputStream& inInputStream )									{ ( void ) inInputStream; }

private:
    /* 坐标 */
    MyVec3 mLocation;
    /* 颜色 */
    MyVec3 mColor;
    /* 碰撞半径 */
    float  mCollisionRadius;
    /* 旋转 */
    float  mRotation;
    /* 缩放比例 */
    float  mScale;
    int    mIndexInWorld;

    bool   mDoesWantToDie;
    /* 网络标识 */
    int    mNetworkId;

};

typedef std::shared_ptr< MyGameObj >	MyGameObjPtr;

#endif
