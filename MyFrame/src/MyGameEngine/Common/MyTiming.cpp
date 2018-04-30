#include "MyGameEngineShared.h"

float kDesiredFrameTime = 0.0333f; ///0.0166f;

#include <chrono>
using namespace std::chrono;


MyTiming	MyTiming::sInstance;

namespace
{
	high_resolution_clock::time_point sStartTime;
}

MyTiming::MyTiming()
{
	sStartTime = high_resolution_clock::now();
}

void MyTiming::Update()
{

	double currentTime = GetTime();

    mDeltaTime = ( float ) ( currentTime - mLastFrameStartTime );

    //frame lock at (kDesiredFrameTime) fps
	while( mDeltaTime < kDesiredFrameTime )
	{
		currentTime = GetTime();

		mDeltaTime = (float)( currentTime - mLastFrameStartTime );
	}
	
	mLastFrameStartTime = currentTime;
	mFrameStartTimef = static_cast< float > ( mLastFrameStartTime );

}

double MyTiming::GetTime() const
{
	auto now = high_resolution_clock::now();
	auto ms = duration_cast< milliseconds >( now - sStartTime ).count();
	//a little uncool to then convert into a double just to go back, but oh well.
	return static_cast< double >( ms ) / 1000;
}
