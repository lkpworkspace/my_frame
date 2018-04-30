#ifndef __MyMath_H__
#define __MyMath_H__

#include <math.h>

class MyVec3
{
public:

    float		mX, mY, mZ;

    MyVec3( float x, float y, float z ) :
        mX( x ),
        mY( y ),
        mZ( z )
    {}

    MyVec3() :
        mX( 0.0f ),
        mY( 0.0f ),
        mZ( 0.0f )
    {}

    void Set( float x, float y, float z )
    {
        mX = x;
        mY = y;
        mZ = z;
    }

    friend MyVec3 operator+( const MyVec3& inLeft, const MyVec3& inRight )
    {
        return MyVec3( inLeft.mX + inRight.mX, inLeft.mY + inRight.mY, inLeft.mZ + inRight.mZ );
    }

    friend MyVec3 operator-( const MyVec3& inLeft, const MyVec3& inRight )
    {
        return MyVec3( inLeft.mX - inRight.mX, inLeft.mY - inRight.mY, inLeft.mZ - inRight.mZ );
    }

    // Component-wise multiplication
    friend MyVec3 operator*( const MyVec3& inLeft, const MyVec3& inRight )
    {
        return MyVec3( inLeft.mX * inRight.mX, inLeft.mY * inRight.mY, inLeft.mZ * inRight.mZ );
    }

    // Scalar multiply
    friend MyVec3 operator*( float inScalar, const MyVec3& inVec )
    {
        return MyVec3( inVec.mX * inScalar, inVec.mY * inScalar, inVec.mZ * inScalar );
    }

    friend MyVec3 operator*( const MyVec3& inVec, float inScalar )
    {
        return MyVec3( inVec.mX * inScalar, inVec.mY * inScalar, inVec.mZ * inScalar );
    }

    MyVec3& operator*=( float inScalar )
    {
        mX *= inScalar;
        mY *= inScalar;
        mZ *= inScalar;
        return *this;
    }

    MyVec3& operator+=( const MyVec3& inRight )
    {
        mX += inRight.mX;
        mY += inRight.mY;
        mZ += inRight.mZ;
        return *this;
    }

    MyVec3& operator-=( const MyVec3& inRight )
    {
        mX -= inRight.mX;
        mY -= inRight.mY;
        mZ -= inRight.mZ;
        return *this;
    }

    float Length()
    {
        return sqrtf( mX * mX + mY * mY + mZ * mZ );
    }

    float LengthSq()
    {
        return mX * mX + mY * mY + mZ * mZ;
    }

    float Length2D()
    {
        return sqrtf( mX * mX + mY * mY );
    }

    float LengthSq2D()
    {
        return mX * mX + mY * mY;
    }

    void Normalize()
    {
        float length = Length();
        mX /= length;
        mY /= length;
        mZ /= length;
    }

    void Normalize2D()
    {
        float length = Length2D();
        mX /= length;
        mY /= length;
    }

    friend float Dot( const MyVec3& inLeft, const MyVec3& inRight )
    {
        return ( inLeft.mX * inRight.mX + inLeft.mY * inRight.mY + inLeft.mZ * inRight.mZ );
    }

    friend float Dot2D( const MyVec3& inLeft, const MyVec3& inRight )
    {
        return ( inLeft.mX * inRight.mX + inLeft.mY * inRight.mY );
    }

    friend MyVec3 Cross( const MyVec3& inLeft, const MyVec3& inRight )
    {
        MyVec3 temp;
        temp.mX = inLeft.mY * inRight.mZ - inLeft.mZ * inRight.mY;
        temp.mY = inLeft.mZ * inRight.mX - inLeft.mX * inRight.mZ;
        temp.mZ = inLeft.mX * inRight.mY - inLeft.mY * inRight.mX;
        return temp;
    }

    friend MyVec3 Lerp( const MyVec3& inA, const MyVec3& inB, float t )
    {
        return MyVec3( inA + t * ( inB - inA ) );
    }

    static const MyVec3 Zero;
    static const MyVec3 UnitX;
    static const MyVec3 UnitY;
    static const MyVec3 UnitZ;
};


class MyQuaternion
{
public:

    float		mX, mY, mZ, mW;

};


template< int tValue, int tBits >
struct GetRequiredBitsHelper
{
    enum { Value = GetRequiredBitsHelper< ( tValue >> 1 ), tBits + 1 >::Value };
};

template< int tBits >
struct GetRequiredBitsHelper< 0, tBits >
{
    enum { Value = tBits };
};

template< int tValue >
struct GetRequiredBits
{
    enum { Value = GetRequiredBitsHelper< tValue, 0 >::Value };
};

namespace MyMath
{
    const float PI = 3.1415926535f;
    float GetRandomFloat();

    MyVec3 GetRandomVector( const MyVec3& inMin, const MyVec3& inMax );

    inline bool	Is2DVectorEqual( const MyVec3& inA, const MyVec3& inB )
    {
        return ( inA.mX == inB.mX && inA.mY == inB.mY );
    }

    inline float ToDegrees( float inRadians )
    {
        return inRadians * 180.0f / PI;
    }
}

namespace MyColors
{
    static const MyVec3 Black( 0.0f, 0.0f, 0.0f );
    static const MyVec3 White( 1.0f, 1.0f, 1.0f );
    static const MyVec3 Red( 1.0f, 0.0f, 0.0f );
    static const MyVec3 Green( 0.0f, 1.0f, 0.0f );
    static const MyVec3 Blue( 0.0f, 0.0f, 1.0f );
    static const MyVec3 LightYellow( 1.0f, 1.0f, 0.88f );
    static const MyVec3 LightBlue( 0.68f, 0.85f, 0.9f );
    static const MyVec3 LightPink( 1.0f, 0.71f, 0.76f );
    static const MyVec3 LightGreen( 0.56f, 0.93f, 0.56f );
}

#endif
