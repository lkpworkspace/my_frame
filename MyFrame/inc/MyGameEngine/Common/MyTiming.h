#ifndef MyMyTiming_H
#define MyMyTiming_H

class MyTiming
{
public:

    MyTiming();
	
	void Update();

	float GetDeltaTime() const { return mDeltaTime; }

	double GetTime() const;

	float GetTimef() const
	{
		return static_cast< float >( GetTime() );
	}

	float GetFrameStartTime() const { return mFrameStartTimef; }


    static MyTiming sInstance;

private:
	float		mDeltaTime;
	uint64_t	mDeltaTick;

	double		mLastFrameStartTime;
	float		mFrameStartTimef;
	double		mPerfCountDuration;

};

#endif
