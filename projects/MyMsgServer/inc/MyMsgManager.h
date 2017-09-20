#ifndef MYMSGMANAGER_H
#define MYMSGMANAGER_H
#include "MyMsgCommon.h"
#include "MyControls.h"
#include "MyMsgConnect.h"

class MyMsgManager
{
    typedef MyMap<std::string, MyMsgConnect*> MyMsgGroup_t;
    typedef MyMap<std::string, MyMsgGroup_t*> MyMsgServer_t;
    typedef MyMap<std::string, MyMsgServer_t*> MyMsgServers_t;
public:
    static MyMsgManager* GetInstance();
    static void Destory();

    bool GetConnect(MyMsgConnect*){}
    bool InsertConnect(MyMsgConnect*);
    bool RemoveConnect(MyMsgConnect*);
private:
    MyMsgManager();
    void RdLock();
    void WrLock();
    void UnLock();
    MyMsgGroup_t* GetGroup(std::string servName, std::string name);
    MyMsgServer_t* GetServer(std::string name);

    static MyMsgManager* instance;
    MyMsgServers_t m_servers;
};

#endif // MYMSGMANAGER_H
