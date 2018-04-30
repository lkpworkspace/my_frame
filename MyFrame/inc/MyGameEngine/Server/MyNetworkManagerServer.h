#ifndef MyNetworkManagerServer_H
#define MyNetworkManagerServer_H

class MyNetworkManagerServer : public MyNetworkManager
{
public:
    static bool StaticInit();
    static MyNetworkManagerServer* sInstance;

    /// Handle
    virtual void	ProcessPacket( MyInputStream* inGameMsg);
    /// Send outgoing packet
    void SendOutgoingPackets();

    MyGameObj* RegisterGameObject( MyGameObj* inGameObject );
    void UnregisterGameObject(MyGameObj* inGameObject);

    void SetStateDirty( int inNetworkId, uint32_t inDirtyState );

    MyClientProxy* GetClientProxy(int inNetworkId) const;
private:
    MyNetworkManagerServer();

    /// Handle
    void HandleHelloPack(MyGameMsg *inGameMsg, MyClientProxy* inClient);
    void HandleInputPack(MyGameMsg* inGameMsg, MyClientProxy* inClient);
    void HandleNewClient( MyClientProxy* inClientProxy );

    void HandleClientDisconnected( MyClientProxy* inClientProxy );
    void HandleLostClient( MyClientProxy* inClientProxy );

    /// send outgoing packet
    void SendWelcomePack(MyClientProxy* inClient);
    void SendStatePacketToClient( MyClientProxy* inClientProxy );


private:
    int				mNewPlayerId;
    int				mNewNetworkId;

    typedef unordered_map< int, MyClientProxy* > IntToClientMap_t;
    IntToClientMap_t mPlayerIdToClientMap;
};

#endif
