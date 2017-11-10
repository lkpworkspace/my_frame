#include "MyMsgManager.h"
#include "MyMsgConnect.h"

MyMsgManager::MyMsgManager()
{
    pthread_rwlock_init(&m_rw_mutex,NULL);
    std::vector<std::string>* row;
    // 搜索服务器
    GetMySqlite3()->ExecSql("select distinct msg_server from mymsgaccount");
    std::vector<std::string> servers;
    while((row = GetMySqlite3()->GetRow()) != NULL)
    {
        auto begin = row->begin();
        std::string server_name = begin[0];
        servers.push_back(server_name);
        MyMsgServer_t* server = new MyMsgServer_t();
        MyMsgMemServer_t* memgroup = new MyMsgMemServer_t();
        m_servers.Insert(server_name,server);
        m_mem_servers.Insert(server_name,memgroup);
        MyDebugPrint("Get \" %s \" server\n",begin[0].c_str());
    }
    // 搜索服务器下的所有组
    for(int i = 0; i < servers.size(); ++i)
    {
        std::string sql = "select distinct msg_group from mymsgaccount where msg_server='";
        sql += servers[i];
        sql += "'";
        GetMySqlite3()->ExecSql(sql);
        while((row = GetMySqlite3()->GetRow()) != NULL)
        {
            auto begin = row->begin();
            MyMsgServer_t* s = GetServer(servers[i]);
            MyMsgMemServer_t* s2 = m_mem_servers.Get(servers[i]);
            if(s)
            {
                MyMsgGroup_t* group = new MyMsgGroup_t();
                s->Insert(begin[0],group);
            }
            if(s2)
            {
                MyMsgMembers_t* members = new MyMsgMembers_t();
                s2->Insert(begin[0],members);
            }
            MyDebugPrint("Insert to \" %s \" server: \" %s \" group\n",servers[i].c_str(),begin[0].c_str());
        }
    }
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

std::vector<AccountInfo_t*>* MyMsgManager::GetGroupMembers(
        std::string serv,
        std::string group)
{
    MySqlite3* db = GetMySqlite3();
    MyMsgMembers_t* g = m_mem_servers.Get(serv)->Get(group);
    std::vector<AccountInfo_t*>::iterator begin = g->begin();
    std::vector<AccountInfo_t*>::iterator end = g->end();

    if(g->size() > 0)
    {
        return g;
    }
    std::string sql = "select * from mymsgaccount where msg_server='";
    sql += serv;
    sql += "' and msg_group='";
    sql += group;
    sql += "'";
    db->ExecSql(sql);
    std::vector<std::string>* row;
    while((row = db->GetRow()) != NULL)
    {
        auto b = row->begin();
        AccountInfo_t* account = new AccountInfo_t();
        account->m_id = b[0];
        account->m_pass = b[1];
        account->m_name = b[2];
        account->m_group = b[3];
        account->m_lv = b[4];
        account->m_server = b[5];
        g->push_back(account);
    }
    MyDebugPrint("Get %s:%s members info from database\n", serv.c_str(), group.c_str());
    return g;
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
                begin++;
            }
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
    MyMsgServer_t* server = GetServer(servName);
    if(server->Find(name))
    {
        return server->Get(name);
    }
    return NULL;
}

MyMsgManager::MyMsgServer_t *MyMsgManager::GetServer(std::string name)
{
    if(m_servers.Find(name))
    {
        return m_servers.Get(name);
    }
    return NULL;
}
















