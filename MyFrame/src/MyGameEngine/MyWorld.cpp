#include "MyGameEngineShared.h"

MyWorld* MyWorld::sInstance = nullptr;

void MyWorld::StaticInit()
{
    if(sInstance == nullptr)
        sInstance =  new MyWorld();
}


void MyWorld::RegisterCreationFunction( uint32_t inFourCCName, MyGameObjCreateFunc inCreationFunction )
{
    mNameToGameObjCreationFunctionMap[ inFourCCName ] = inCreationFunction;
}

MyGameObj* MyWorld::CreateGameObject( uint32_t inFourCCName )
{
    MyGameObjCreateFunc creationFunc = mNameToGameObjCreationFunctionMap[ inFourCCName ];

    MyGameObj* gameObject = creationFunc();

    AddGameObject( gameObject );

    return gameObject;
}

void MyWorld::AddGameObject( MyGameObj* inGameObject )
{
    mGameObjects.push_back( inGameObject );
    inGameObject->SetIndexInWorld( mGameObjects.size() - 1 );
}

void MyWorld::RemoveGameObject( MyGameObj* inGameObject )
{
    int index = inGameObject->GetIndexInWorld();

    int lastIndex = mGameObjects.size() - 1;
    if( index != lastIndex )
    {
        mGameObjects[ index ] = mGameObjects[ lastIndex ];
        mGameObjects[ index ]->SetIndexInWorld( index );
    }

    inGameObject->SetIndexInWorld( -1 );
    mGameObjects.pop_back();
}

void MyWorld::Update()
{
    for( int i = 0, c = mGameObjects.size(); i < c; ++i )
    {
        MyGameObj* go = mGameObjects[ i ];


        if( !go->DoesWantToDie() )
        {
            go->Update();
        }
        //you might suddenly want to die after your update, so check again
        if( go->DoesWantToDie() )
        {
            RemoveGameObject( go );
            go->HandleDying();
            --i;
            --c;
        }
    }
}
