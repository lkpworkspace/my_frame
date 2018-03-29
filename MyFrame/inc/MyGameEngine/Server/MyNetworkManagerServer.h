#ifndef MyNetworkManagerServer_H
#define MyNetworkManagerServer_H

class MyNetworkManagerServer : public MyNetworkManager
{
public:
    static bool StaticInit();
    static MyNetworkManagerServer* sInstance;

    virtual void	ProcessPacket( MyInputStream* inGameMsg);
    void SendOutgoingPackets();

    MyGameObj* RegisterGameObject( MyGameObj* inGameObject );
    void SetStateDirty( int inNetworkId, uint32_t inDirtyState );
private:
    MyNetworkManagerServer();

    void HandleHelloPack(MyGameMsg *inGameMsg, MyClientProxy* inClient);
    void HandleInputPack(MyGameMsg* inGameMsg, MyClientProxy* inClient);
    void SendWelcomePack(MyClientProxy* inClient);

    void SendStatePacketToClient( MyClientProxy* inClientProxy );
    void HandleNewClient( MyClientProxy* inClientProxy );
    void HandleLostClient( MyClientProxy* inClientProxy );

private:
    int				mNewPlayerId;
    int				mNewNetworkId;


    typedef unordered_map< int, MyClientProxy* > IntToClientMap_t;
    IntToClientMap_t mPlayerIdToClientMap;
};

#endif
