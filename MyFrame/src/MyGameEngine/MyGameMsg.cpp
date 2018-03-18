#include "MyGameEngineShared.h"

myframe::MyEvent* MyGameMsg::StaticCreate()
{
    return (myframe::MyEvent*)(new MyGameMsg());
}

