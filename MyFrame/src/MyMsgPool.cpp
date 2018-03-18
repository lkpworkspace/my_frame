#include "MyMsgPool.h"

USING_MYFRAME;

//MyMsgPool* MyMsgPool::l_instance = nullptr;

//MyMsgPool* MyMsgPool::Instance()
//{
//    if(l_instance == nullptr)
//    {
//        l_instance = new MyMsgPool();
//    }
//    return l_instance;
//}

bool MyMsgPool::RegMsg(std::string name, MyMsgCreateFunc regfunc)
{
    m_mutex.lock();
    if(m_msgs_create.find(name) != m_msgs_create.end())
    {
        m_mutex.unlock();
        return false;
    }
    m_msgs_create[name] = regfunc;
    m_mutex.unlock();
    return true;
}

bool MyMsgPool::UnregMsg(std::string name)
{
    //TODO(lkp): ...
    m_mutex.lock();
    m_mutex.unlock();
    return false;
}

MyEvent* MyMsgPool::Get(std::string name)
{
    MyEvent* temp = nullptr;

    m_mutex.lock();
    if(m_free_msgs.find(name) != m_free_msgs.end())
        if(!m_free_msgs[name].empty())
        {
            temp = m_free_msgs[name].front();
            m_free_msgs[name].pop_front();
            m_mutex.unlock();
            return temp;
        }
    temp = (m_msgs_create[name])();
    m_mutex.unlock();
    return temp;
}

void MyMsgPool::Free(MyEvent* msg)
{
    m_mutex.lock();
    m_free_msgs[msg->GetObjName()].push_back(msg);
    m_mutex.unlock();
}
