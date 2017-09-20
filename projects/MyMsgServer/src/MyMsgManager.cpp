#include "MyMsgManager.h"

MyMsgManager* MyMsgManager::instance = NULL;
pthread_rwlock_t m_rw_mutex = PTHREAD_RWLOCK_INITIALIZER;

MyMsgManager* MyMsgManager::GetInstance()
{
    if(instance == NULL)
        instance = new MyMsgManager();
    return instance;
}

void MyMsgManager::Destory()
{
    pthread_rwlock_destroy(&m_rw_mutex);
}

void MyMsgManager::RdLock()
{
    pthread_rwlock_rdlock(&m_rw_mutex);
}

void MyMsgManager::WrLock()
{
    pthread_rwlock_wrlock(&m_rw_mutex);
}

void MyMsgManager::UnLock()
{
    pthread_rwlock_unlock(&m_rw_mutex);
}



bool MyMsgManager::InsertConnect(MyMsgConnect* c)
{
    MyMsgGroup_t* group;
    bool ret = false;
    if(c != NULL)
    {
        WrLock();
        group = GetGroup(c->m_server,c->m_group);
        ret =  group->Insert(c->m_id,c);
        UnLock();
    }
    return ret;
}

bool MyMsgManager::RemoveConnect(MyMsgConnect *c)
{
    // so diffcult
    return false;
}

MyMsgManager::MyMsgManager()
{
}

MyMsgManager::MyMsgGroup_t *MyMsgManager::GetGroup(std::string servName, std::string name)
{
    MyMsgGroup_t* group;
    MyMsgServer_t* server = GetServer(servName);
    if(server->Find(name))
    {
        return server->Get(name);
    }
    group = new MyMsgGroup_t();
    if(server->Insert(name,group))
        return group;
    return NULL;
}

MyMsgManager::MyMsgServer_t *MyMsgManager::GetServer(std::string name)
{
    MyMsgServer_t* server;
    if(m_servers.Find(name))
    {
        return m_servers.Get(name);
    }
    server = new MyMsgServer_t();
    if(m_servers.Insert(name,server))
        return server;
    return NULL;
}
















