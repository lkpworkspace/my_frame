#ifndef MyNetworkManagerServer_H
#define MyNetworkManagerServer_H

class MyNetworkManagerServer : public MyNetworkManager
{
public:
    static bool StaticInit();
    static MyNetworkManagerServer* sInstance;

    virtual void	ProcessPacket( MyInputStream* inGameMsg);
    void SendOutgoingPackets();
private:
    MyNetworkManagerServer();

    void HandleHelloPack(MyGameMsg *inGameMsg, MyClientProxy* inClient);
    void HandleInputPack(MyGameMsg* inGameMsg, MyClientProxy* inClient);
    void SendWelcomePack(MyClientProxy* inClient);

private:
    int				mNewPlayerId;
    int				mNewNetworkId;

    typedef unordered_map< int, MyGameObj* > IntToGameObjectMap_t;
    typedef unordered_map< int, MyClientProxy* > IntToClientMap_t;
    IntToGameObjectMap_t mNetworkIdToGameObjectMap;
    IntToClientMap_t mPlayerIdToClientMap;
};

#endif
