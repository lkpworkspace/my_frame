#ifndef __MyIOStream_H__
#define __MyIOStream_H__

#include <string.h>

#include <cstdint>
#include <cstdlib>
#include <string>

#include "MyMath.h"
#include "MyEvent.h"

/**
 * NOTE THAT: 类中的输入输出操作均是采用bit流的方式操作， 注意长度是比特
 */

inline uint32_t ConvertToFixed( float inNumber, float inMin, float inPrecision )
{
    return static_cast< int > ( ( inNumber - inMin ) / inPrecision );
}

inline float ConvertFromFixed( uint32_t inNumber, float inMin, float inPrecision )
{
    return static_cast< float >( inNumber ) * inPrecision + inMin;
}


class MyOutputStream : public myframe::MyEvent
{
public:

    MyOutputStream() :
        mBitHead(0),
        mBuffer(nullptr)
    {
        ReallocBuffer( 1500 * 8 );
    }

    ~MyOutputStream()	{ std::free( mBuffer ); }

    /* 重写 MyEvent 的虚函数 */
    virtual EVENT_TYPE GetEventType() override { return EV_MSG; }
    /* MyEvent 对象的回调函数 */
    virtual void* CallBackFunc(MyEvent*) override { return nullptr; }

    void		WriteBits( uint8_t inData, uint32_t inBitCount );
    void		WriteBits( const void* inData, uint32_t inBitCount );

    const 	char*	GetBufferPtr()		const	{ return mBuffer; }
    uint32_t		GetBitLength()		const	{ return mBitHead; }
    uint32_t		GetByteLength()		const	{ return ( mBitHead + 7 ) >> 3; }

    void WriteBytes( const void* inData, uint32_t inByteCount )	{ WriteBits( inData, inByteCount << 3 ); }

    /*
    void Write( uint32_t inData, uint32_t inBitCount = 32 )	{ WriteBits( &inData, inBitCount ); }
    void Write( int inData, uint32_t inBitCount = 32 )		{ WriteBits( &inData, inBitCount ); }
    void Write( float inData )								{ WriteBits( &inData, 32 ); }

    void Write( uint16_t inData, uint32_t inBitCount = 16 )	{ WriteBits( &inData, inBitCount ); }
    void Write( int16_t inData, uint32_t inBitCount = 16 )	{ WriteBits( &inData, inBitCount ); }

    void Write( uint8_t inData, uint32_t inBitCount = 8 )	{ WriteBits( &inData, inBitCount ); }
    */

    template< typename T >
    void Write( T inData, uint32_t inBitCount = sizeof( T ) * 8 )
    {
        static_assert( std::is_arithmetic< T >::value ||
                      std::is_enum< T >::value,
                      "Generic Write only supports primitive data types" );
        WriteBits( &inData, inBitCount );
    }

    void 		Write( bool inData )								{ WriteBits( &inData, 1 ); }

    void		Write( const MyVec3& inVector );
    void		Write( const MyQuaternion& inQuat );

    void Write( const std::string& inString )
    {
        uint32_t elementCount = static_cast< uint32_t >( inString.size() );
        Write( elementCount );
        for( const auto& element : inString )
        {
            Write( element );
        }
    }

private:
    void		ReallocBuffer( uint32_t inNewBitCapacity );

    char*		mBuffer;
    uint32_t	mBitHead;
    uint32_t	mBitCapacity;
};

/**
 * @brief The MyInputStream class
 * 继承 MyEvent 可以使得该对象可以进行线程间传递消息
 */
class MyInputStream : public myframe::MyEvent
{
public:
    MyInputStream() :
        mBitCapacity( 0 ),
        mBitHead( 0 ),
        mBuffer( nullptr ),
        mIsBufferOwner( true )
    {}

    MyInputStream( const char* inBuffer, uint32_t inBitCount ) :
        mBitCapacity( inBitCount ),
        mBitHead( 0 ),
        mIsBufferOwner( true )
    {
        int byteCount = mBitCapacity / 8;
        mBuffer = static_cast< char* >( malloc( byteCount ) );
        memcpy( mBuffer, inBuffer, byteCount );
    }

    MyInputStream( const MyInputStream& inOther ) :
        mBitCapacity( inOther.mBitCapacity ),
        mBitHead( inOther.mBitHead ),
        mIsBufferOwner( true )
    {
        //allocate buffer of right size
        int byteCount = mBitCapacity / 8;
        mBuffer = static_cast< char* >( malloc( byteCount ) );
        //copy
        memcpy( mBuffer, inOther.mBuffer, byteCount );
    }

    ~MyInputStream()	{ if( mIsBufferOwner ) { free( mBuffer ); }; }

    /* 重写 MyEvent 的虚函数 */
    virtual EVENT_TYPE GetEventType() override { return EV_MSG; }
    /* MyEvent 对象的回调函数 */
    virtual void* CallBackFunc(MyEvent*) override { return nullptr; }

    /* 获得buf指针 */
    const 	char*	GetBufferPtr()		const	{ return mBuffer; }
    /* 获得buf长度 */
    uint32_t		GetBitLength()		const	{ return mBitCapacity; }
    uint32_t		GetByteLength()		const	{ return ( mBitCapacity + 7 ) >> 3; }
    /* 获得剩余的空间 */
    uint32_t	GetRemainingBitCount() 	const { return mBitCapacity - mBitHead; }
    /* 重置空间大小，下标归0(真正大小不会改变) */
    void		ResetToCapacity( uint32_t inByteCapacity )				{ mBitCapacity = inByteCapacity << 3; mBitHead = 0; }
    /* 设置要读的buf */
    void SetBuffer(const char *inBuffer, uint32_t inBitCount);

    /* read */
    void		ReadBits( uint8_t& outData, uint32_t inBitCount );
    void		ReadBits( void* outData, uint32_t inBitCount );
    void		ReadBytes( void* outData, uint32_t inByteCount )		{ ReadBits( outData, inByteCount << 3 ); }

    template< typename T >
    void Read( T& inData, uint32_t inBitCount = sizeof( T ) * 8 )
    {
        static_assert( std::is_arithmetic< T >::value ||
                      std::is_enum< T >::value,
                      "Generic Read only supports primitive data types" );
        ReadBits( &inData, inBitCount );
    }
    /*
    void		Read( uint32_t& outData, uint32_t inBitCount = 32 )		{ ReadBits( &outData, inBitCount ); }
    void		Read( int& outData, uint32_t inBitCount = 32 )			{ ReadBits( &outData, inBitCount ); }
    void		Read( float& outData )									{ ReadBits( &outData, 32 ); }

    void		Read( uint16_t& outData, uint32_t inBitCount = 16 )		{ ReadBits( &outData, inBitCount ); }
    void		Read( int16_t& outData, uint32_t inBitCount = 16 )		{ ReadBits( &outData, inBitCount ); }

    void		Read( uint8_t& outData, uint32_t inBitCount = 8 )		{ ReadBits( &outData, inBitCount ); }
    */

    void		Read( bool& outData )									{ ReadBits( &outData, 1 ); }

    void		Read( MyQuaternion& outQuat );

    void Read( std::string& inString )
    {
        uint32_t elementCount;
        Read( elementCount );
        inString.resize( elementCount );
        for( auto& element : inString )
        {
            Read( element );
        }
    }

    void Read( MyVec3& inVector );

private:
    char*		mBuffer;
    uint32_t	mBitHead;
    uint32_t	mBitCapacity;
    bool		mIsBufferOwner;

};

#endif
