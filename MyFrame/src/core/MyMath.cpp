#include "MyMath.h"
#include <random>

const MyVec3 MyVec3::Zero( 0.0f, 0.0f, 0.0f );
const MyVec3 MyVec3::UnitX( 1.0f, 0.0f, 0.0f );
const MyVec3 MyVec3::UnitY( 0.0f, 1.0f, 0.0f );
const MyVec3 MyVec3::UnitZ( 0.0f, 0.0f, 1.0f );

float MyMath::GetRandomFloat()
{
    static std::random_device rd;
    static std::mt19937 gen( rd() );
    static std::uniform_real_distribution< float > dis( 0.f, 1.f );
    return dis( gen );
}

MyVec3 MyMath::GetRandomVector( const MyVec3& inMin, const MyVec3& inMax )
{
    MyVec3 r = MyVec3( GetRandomFloat(), GetRandomFloat(), GetRandomFloat() );
    return inMin + ( inMax - inMin ) * r;
}
