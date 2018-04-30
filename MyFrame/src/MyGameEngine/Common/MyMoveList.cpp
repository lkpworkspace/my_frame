#include "MyGameEngineShared.h"

const MyMove& MyMoveList::AddMove( const MyInputState& inInputState, float inTimestamp )
{
	//first move has 0 time. it's okay, it only happens once
	float deltaTime = mLastMoveTimestamp >= 0.f ? inTimestamp - mLastMoveTimestamp : 0.f;

	mMoves.emplace_back( inInputState, inTimestamp, deltaTime );

	mLastMoveTimestamp = inTimestamp;

	return mMoves.back();
}

bool MyMoveList::AddMove( const MyMove& inMove )
{
	//adjust the delta time and then place!
	float timeStamp = inMove.GetTimestamp();
	float deltaTime = mLastMoveTimestamp >= 0.f ? timeStamp - mLastMoveTimestamp : 0.f;
	mLastMoveTimestamp = timeStamp;
	mMoves.emplace_back( inMove.GetInputState(), timeStamp, deltaTime );
	return true;
}

void	MyMoveList::RemovedProcessedMoves( float inLastMoveProcessedOnServerTimestamp )
{
	while( !mMoves.empty() && mMoves.front().GetTimestamp() <= inLastMoveProcessedOnServerTimestamp )
	{
		mMoves.pop_front();
	}
}
