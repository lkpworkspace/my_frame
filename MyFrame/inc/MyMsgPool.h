#ifndef MYEVENTPOOL_H
#define MYEVENTPOOL_H
#include <unordered_map>
#include <list>
#include "MyEvent.h"



NS_MYFRAME_BEGIN
/**
    概述:     该类为框架之间进行消息传输定义的保存消息对象的消息池
    用途:     避免消息对象频繁的创建删除造成额外的开销
    注意事项:  消息类必须继承 MyEvent
              消息名以 MYMSG_ 开头
    子类需要实现的方法:
        static MyEvent* Create();

*/

typedef MyEvent* (*MyMsgCreate)();
typedef std::list<MyEvent*> MyMsgList_t;
class MyMsgPool
{
public:
    static MyMsgPool* Instance();
    bool RegMsg(std::string name, MyMsgCreate regfunc);
    bool UnregMsg(std::string name);
    MyEvent* Get(std::string name);
    void Free(MyEvent* msg);
protected:
    MyMsgPool(){}
    ~MyMsgPool(){}
private:
    std::unordered_map<std::string, MyMsgCreate> m_msgs_create;
    std::unordered_map<std::string, MyMsgList_t> m_free_msgs;
    static MyMsgPool* l_instance;
};

NS_MYFRAME_END // namespace
#endif // MYEVENTPOOL_H
