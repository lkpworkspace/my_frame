#include "MyKeyEvent.h"
#include "MyAllEvent.h"

using namespace my_master;
MyKeyEvent::MyKeyEvent()
{

}

MyKeyEvent::~MyKeyEvent()
{

}

MyEvent::EVENT_TYPE MyKeyEvent::GetEventType()
{
    return MyEvent::FILEFD;
}

MyEvent::CLASS_TYPE MyKeyEvent::GetClassType()
{
    return MyEvent::KEY;
}

uint32_t MyKeyEvent::GetEpollEventType()
{
    return EPOLLIN;
}

void* MyKeyEvent::CallBackFunc(MyEvent*)
{
    MyAllEvent::BoardEvent(this);
}

int MyKeyEvent::GetKey()
{
    // TODO...
    return 0;
}

std::string MyKeyEvent::GetDevPath()
{
    // TODO...
    return std::string("");
}
