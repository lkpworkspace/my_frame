#include "../inc/MyMsgManager.h"

MyMsgManager* MyMsgManager::instance = NULL;

MyMsgManager* MyMsgManager::GetInstance()
{
    if(instance == NULL)
        instance = new MyMsgManager();
    return instance;
}

void MyMsgManager::InsertGroup(std::string name)
{

}

void MyMsgManager::RemoveGroup(std::string name)
{

}

void MyMsgManager::InsertConnect(MyConnect* c)
{

}

void MyMsgManager::RemoveConnect(MyConnect* c)
{

}
