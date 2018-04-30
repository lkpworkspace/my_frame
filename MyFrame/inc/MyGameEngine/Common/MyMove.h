#ifndef MyMove_H
#define MyMove_H

class MyMove
{
public:

    MyMove() {}

    MyMove( const MyInputState& inInputState, float inTimestamp, float inDeltaTime ) :
		mInputState( inInputState ),
		mTimestamp( inTimestamp ),
		mDeltaTime( inDeltaTime )
	{}


    const MyInputState&	GetInputState()	const		{ return mInputState; }
	float				GetTimestamp()	const		{ return mTimestamp; }
	float				GetDeltaTime()	const		{ return mDeltaTime; }

    bool Write(MyOutputStream &inOutputStream ) const;
    bool Read(MyInputStream &inInputStream );

private:
    MyInputState	mInputState;
    float           mTimestamp;
    float           mDeltaTime;

};

#endif
