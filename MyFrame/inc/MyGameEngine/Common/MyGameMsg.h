#ifndef MyGameMsg_H
#define MyGameMsg_H

class MyGameMsg : public MyInputStream
{
public:
    static myframe::MyEvent* StaticCreate();

    void* GetClientProxy(){ return mClient; }
    void SetClientProxy(void* inClient){ mClient = inClient; }

private:

    void* mClient;
};

#endif
