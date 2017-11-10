#ifndef MYMSGMANAGER_H
#define MYMSGMANAGER_H
#include "MyMsgCommon.h"
#include "MyControls.h"

class MyMsgManager
{
    typedef MyMap<std::string, MyMsgConnect*> MyMsgGroup_t;
    typedef MyMap<std::string, MyMsgGroup_t*> MyMsgServer_t;
    typedef MyMap<std::string, MyMsgServer_t*> MyMsgServers_t;
    typedef std::vector<AccountInfo_t*> MyMsgMembers_t;
    typedef MyMap<std::string, MyMsgMembers_t*> MyMsgMemServer_t;
    typedef MyMap<std::string, MyMsgMemServer_t*> MyMsgMemServers_t;

public:
    MyMsgManager();
    ~MyMsgManager();

    void RdLock();
    void WrLock();
    void UnLock();

    // 存储所有的帐号
    std::vector<AccountInfo_t *> *GetGroupMembers(std::string serv, std::string group);

    // 存储在线的帐号
    MyMsgConnect* GetConnect(std::string serv, std::string group, std::string name);
    bool InsertConnect(MyMsgConnect*);
    bool RemoveConnect(MyMsgConnect*);
    std::vector<MyMsgConnect*> GetGroup(MyMsgConnect*);
private:
    MyMsgServer_t* GetServer(std::string name);
    MyMsgGroup_t* GetGroup(std::string servName, std::string name);
    // online account
    MyMsgServers_t m_servers;
    // all account
    MyMsgMemServers_t m_mem_servers;
    pthread_rwlock_t m_rw_mutex;
};

#endif // MYMSGMANAGER_H
