#ifndef __GMsg_H__
#define __GMsg_H__

/**
 * @brief The GMsg class
 *      用于线程间传递消息使用的类
 */

class GMsg : public MyInputStream
{
public:
    static myframe::MyEvent* StaticCreate();

    void* GetClientProxy(){ return mClient; }
    void SetClientProxy(void* inClient){ mClient = inClient; }

private:
    void* mClient;
};

class GReplyMsg : public MyOutputStream
{
public:
    static myframe::MyEvent* StaticCreate();

    void* GetClientProxy(){ return mClient; }
    void SetClientProxy(void* inClient){ mClient = inClient; }

private:
    void* mClient;
};


#endif
