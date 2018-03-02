#include "MyFrame.h"
#include "MyIOStream.h"
#include<memory>

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


            void		SetColor( const MyVec3& inColor )					{ mColor = inColor; }
    const MyVec3&		GetColor()					const				{ return mColor; }

            bool		DoesWantToDie()				const				{ return mDoesWantToDie; }
            void		SetDoesWantToDie( bool inWants )				{ mDoesWantToDie = inWants; }

            int			GetNetworkId()				const				{ return mNetworkId; }
            void		SetNetworkId( int inNetworkId );

    virtual uint32_t	Write( MyOutputStream& inOutputStream, uint32_t inDirtyState ) const	{  ( void ) inOutputStream; ( void ) inDirtyState; return 0; }
    virtual void		Read( MyInputStream& inInputStream )									{ ( void ) inInputStream; }

private:

    MyVec3 mLocation;
    MyVec3 mColor;

    float  mCollisionRadius;


    float  mRotation;
    float  mScale;
    int    mIndexInWorld;

    bool   mDoesWantToDie;

    int    mNetworkId;

};

typedef std::shared_ptr< MyGameObj >	MyGameObjPtr;
