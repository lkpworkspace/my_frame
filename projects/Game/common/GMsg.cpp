#include "MyIOStream.h"
#include "IGClientProxy.h"
#include "GMsg.h"

myframe::MyEvent* GMsg::StaticCreate()
{
    return (myframe::MyEvent*)(new GMsg());
}

myframe::MyEvent* GReplyMsg::StaticCreate()
{
    return (myframe::MyEvent*)(new GReplyMsg());
}
