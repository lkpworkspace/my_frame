#include "MyDelegate.h"

MyDelegate* MyDelegate::instance = nullptr;
MyDelegate* MyDelegate::GetInstance()
{
    if(instance == nullptr)
    {
        instance = new MyDelegate;
    }
    return instance;
}

MyDelegate::MyDelegate(QObject *parent) : QObject(parent)
{}

void MyDelegate::AppendMsg(std::string account, std::string msg)
{
    emit appendMsg(account.c_str(),msg.c_str());
}

void MyDelegate::AddFriend(MyAddrInfo info,
               std::string name,
               std::string account,
               std::string mark)
{
    std::string ip = info.GetIp();
    unsigned short port = info.GetPort();
    printf("emit signal\n");
    emit addFriend(ip.c_str(),
                   port,
                   name.c_str(),
                   account.c_str(),
                   mark.c_str());
}
