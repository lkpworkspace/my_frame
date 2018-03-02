#include "MyMsgPool.h"

USING_MYFRAME;

MyMsgPool* MyMsgPool::l_instance = nullptr;

MyMsgPool* MyMsgPool::Instance()
{
    if(l_instance == nullptr)
    {
        l_instance = new MyMsgPool();
    }
    return l_instance;
}

bool MyMsgPool::RegMsg(std::string name, MyMsgCreate regfunc)
{
    if(m_msgs_create.find(name) != m_msgs_create.end())
        return false;
    m_msgs_create[name] = regfunc;
}

bool MyMsgPool::UnregMsg(std::string name)
{
    //TODO(lkp): ...
    return false;
}

MyEvent* MyMsgPool::Get(std::string name)
{
    MyEvent* temp = nullptr;
    if(m_free_msgs.find(name) != m_free_msgs.end())
        if(!m_free_msgs[name].empty())
        {
            temp = m_free_msgs[name].front();
            m_free_msgs[name].pop_front();
            return temp;
        }
    return (m_msgs_create[name])();
}

void MyMsgPool::Free(MyEvent* msg)
{
    m_free_msgs[msg->GetObjName()].push_back(msg);
}
