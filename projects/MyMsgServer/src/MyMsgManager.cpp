#include "MyMsgManager.h"
#include "MyMsgConnect.h"

MyMsgManager::MyMsgManager()
{
    pthread_rwlock_init(&m_rw_mutex,NULL);
}

MyMsgManager::~MyMsgManager()
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
        MyDebugPrint("%s insert to global map\n", c->m_id.c_str());
    }
    return ret;
}

bool MyMsgManager::RemoveConnect(MyMsgConnect *c)
{
    MyMsgConnect* conn;
    MyMsgGroup_t* g;

    WrLock();
    g = m_servers.Get(c->m_server)->Get(c->m_group);
    if(g->Find(c->m_id))
    {
        conn = g->Get(c->m_id);
        if(conn != NULL)
        {
            g->Remove(c->m_id);
            std::map<std::string, MyMsgConnect*>::iterator begin = g->GetMap().begin();
            std::map<std::string, MyMsgConnect*>::iterator end = g->GetMap().end();
            for(;begin != end;)
            {
                begin->second->MemberQuit(c->m_id);
            }
            delete conn;
            conn = NULL;
            MyDebugPrint("%s remove from global map\n", c->m_id.c_str());
        }
    }
    UnLock();
    return true;
}

MyMsgConnect* MyMsgManager::GetConnect(std::string serv, std::string group, std::string name)
{
    MyMsgConnect* conn = NULL;
    MyMsgGroup_t* g = NULL;
    RdLock();
    g = m_servers.Get(serv)->Get(group);
    if(g->Find(name))
        conn = g->Get(name);
    UnLock();
    return conn;
}

std::vector<MyMsgConnect*> MyMsgManager::GetGroup(MyMsgConnect* c)
{
    std::vector<MyMsgConnect*> tempgroup;
    MyMsgGroup_t* g;

    RdLock();
    g = m_servers.Get(c->m_server)->Get(c->m_group);
    std::map<std::string, MyMsgConnect*>::iterator begin = g->GetMap().begin();
    std::map<std::string, MyMsgConnect*>::iterator end = g->GetMap().end();
    for(;begin != end;)
    {
        tempgroup.push_back(begin->second);
    }
    UnLock();
    return tempgroup;
}


////////////////////////////////////////////////////////////////////////////
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
















