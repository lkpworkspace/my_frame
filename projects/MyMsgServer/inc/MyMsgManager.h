#ifndef MYMSGMANAGER_H
#define MYMSGMANAGER_H
#include "MyMsgCommon.h"
#include "MyControls.h"

class MyMsgManager
{
    typedef MyMap<std::string, MyMsgConnect*> MyMsgGroup_t;
    typedef MyMap<std::string, MyMsgGroup_t*> MyMsgServer_t;
    typedef MyMap<std::string, MyMsgServer_t*> MyMsgServers_t;
public:
    MyMsgManager();
    ~MyMsgManager();

    void RdLock();
    void WrLock();
    void UnLock();

    MyMsgConnect* GetConnect(std::string serv, std::string group, std::string name);
    bool InsertConnect(MyMsgConnect*);
    bool RemoveConnect(MyMsgConnect*);
    std::vector<MyMsgConnect*> GetGroup(MyMsgConnect*);
private:
    MyMsgServer_t* GetServer(std::string name);
    MyMsgGroup_t* GetGroup(std::string servName, std::string name);
    MyMsgServers_t m_servers;
    pthread_rwlock_t m_rw_mutex;
};

#endif // MYMSGMANAGER_H
