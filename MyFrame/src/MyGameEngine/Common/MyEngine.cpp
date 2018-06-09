#include "MyGameEngineShared.h"

MyEngine::MyEngine()
{
    MyWorld::StaticInit();
}

MyEngine::~MyEngine()
{
}

/* 2 */
int MyEngine::Run()
{
    MyTiming::sInstance.Update();

    DoFrame();

	return 0;
}

/* 4 */
void MyEngine::DoFrame()
{
    MyWorld::sInstance->Update();
}










