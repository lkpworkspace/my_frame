#ifndef MYMSGMANAGER_H
#define MYMSGMANAGER_H
#include "Comm.h"
#include "MyControls.h"
#include "MyConnect.h"

class MyMsgManager
{
public:
    MyMsgManager* GetInstance();

    void InsertGroup(std::string name);
    void RemoveGroup(std::string name);

    void InsertConnect(MyConnect*);
    void RemoveConnect(MyConnect*);
private:
    MyMsgManager(){}
    static MyMsgManager* instance;
    typedef MyMap<std::string, MyConnect*> MyMsgGroup_t;
    typedef MyMap<std::string, MyMsgGroup_t*> MyMsgGroups_t;
    typedef MyMap<std::string, MyMsgGroup_t*> MyMsgServer_t;
    MyMsgServer_t m_servers;
};

#endif // MYMSGMANAGER_H
