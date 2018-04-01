#ifndef MyGameClient_H
#define MyGameClient_H

class MyGameClient
{
public:
    static MyGameClient* sInstance;
    static bool StaticInit(const std::string& inName, MyAddrInfo& info );

    void CollectEvent(MyList* inEvs);
    void Update();
protected:

    MyGameClient();
private:
};


#endif
