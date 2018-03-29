#include "MyGameEngineShared.h"
#include "MyEngine.h"


MyEngine::MyEngine()
{
    MyWorld::StaticInit();
#if 0
    SocketUtil::StaticInit();

    srand( static_cast< uint32_t >( time( nullptr ) ) );

    GameObjectRegistry::StaticInit();




    ScoreBoardManager::StaticInit();

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
#endif
}

MyEngine::~MyEngine()
{
}

/* 2 */
int MyEngine::Run()
{
    MyTiming::sInstance.Update();

    DoFrame();
#if 0
    // Main message loop
    bool quit = false;
    SDL_Event event;
    memset( &event, 0, sizeof( SDL_Event ) );

    while( !quit && mShouldKeepRunning )
    {
        if( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit = true;
            }
            else
            {
                HandleEvent( &event );
            }
        }
        else
        {
            Timing::sInstance.Update();

            DoFrame();
        }
    }

    return event.type;
#endif
}

/* 4 */
void MyEngine::DoFrame()
{
    MyWorld::sInstance->Update();
}










