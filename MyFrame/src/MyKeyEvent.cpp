#include "../inc/MyKeyEvent.h"
#include "../inc/MyAllEvent.h"

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

std::string MyKeyEvent::GetClassType()
{
    return "MyKeyEvent";
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
