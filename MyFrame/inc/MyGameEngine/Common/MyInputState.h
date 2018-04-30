#ifndef MyInputState_H
#define MyInputState_H

class MyInputState
{
public:

    MyInputState() :
	mDesiredRightAmount( 0 ),
	mDesiredLeftAmount( 0 ),
	mDesiredForwardAmount( 0 ),
	mDesiredBackAmount( 0 ),
	mIsShooting( false )
	{}

	float GetDesiredHorizontalDelta()	const { return mDesiredRightAmount - mDesiredLeftAmount; }
	float GetDesiredVerticalDelta()		const { return mDesiredForwardAmount - mDesiredBackAmount; }
	bool  IsShooting()					const { return mIsShooting; }

    bool Write( MyOutputStream& inOutputStream ) const;
    bool Read( MyInputStream& inInputStream );

private:

	float	mDesiredRightAmount, mDesiredLeftAmount;
	float	mDesiredForwardAmount, mDesiredBackAmount;
	bool	mIsShooting;
};

#endif
