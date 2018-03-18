#ifndef MyMoveList_H
#define MyMoveList_H

class MoveList
{
public:

    typedef deque< MyMove >::const_iterator			const_iterator;
    typedef deque< MyMove >::const_reverse_iterator	const_reverse_iterator;
	
	MoveList():
		mLastMoveTimestamp( -1.f )
	{}
	
    const	MyMove&	AddMove( const MyInputState& inInputState, float inTimestamp );
            bool	AddMove( const MyMove& inMove );

			void	RemovedProcessedMoves( float inLastMoveProcessedOnServerTimestamp );

	float			GetLastMoveTimestamp()	const	{ return mLastMoveTimestamp; } 

    const MyMove&	GetLatestMove()			const	{ return mMoves.back(); }

	void			Clear()							{ mMoves.clear(); }
	bool			HasMoves()				const	{ return !mMoves.empty(); }
	int				GetMoveCount()			const	{ return mMoves.size(); }

	//for for each, we have to match stl calling convention
	const_iterator	begin()					const	{ return mMoves.begin(); }
	const_iterator	end()					const	{ return mMoves.end(); }

    const MyMove&	operator[]( size_t i )	const	{ return mMoves[ i ]; }
private:

	float			mLastMoveTimestamp;
    deque< MyMove >	mMoves;




};


#endif
