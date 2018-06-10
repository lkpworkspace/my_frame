#ifndef __GMsg_H__
#define __GMsg_H__

/**
 * @brief The GMsg class
 *      用于线程间传递消息使用的类
 */
class IGClientProxy;
class GMsg : public MyInputStream
{
public:
    static myframe::MyEvent* StaticCreate();

    IGClientProxy* GetClientProxy(){ return mClient; }
    void SetClientProxy(IGClientProxy* inClient){ mClient = inClient; }

private:
    IGClientProxy* mClient;
};

class GReplyMsg : public MyOutputStream
{
public:
    static myframe::MyEvent* StaticCreate();

    IGClientProxy* GetClientProxy(){ return mClient; }
    void SetClientProxy(IGClientProxy* inClient){ mClient = inClient; }

private:
    IGClientProxy* mClient;
};


#endif
