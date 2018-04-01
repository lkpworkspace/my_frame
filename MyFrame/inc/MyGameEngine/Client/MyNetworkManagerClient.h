#ifndef MyNetworkManagerClient_H
#define MyNetworkManagerClient_H


class MyNetworkManagerClient : public MyNetworkManager
{
    enum NetworkClientState
    {
        NCS_Uninitialized,
        NCS_SayingHello,
        NCS_Welcomed
    };

public:
    static MyNetworkManagerClient* sInstance;
    static void StaticInit(const std::string& inName, MyAddrInfo& inInfo);
    int GetPlayerId() const { return mPlayerId; }
    MySimpleTcpClient& GetClient(){ return mSimpleTcpClient; }

    /// collect event
    void HandleEvent(MyList* inEvs);

    /// recv
    virtual void ProcessPacket(MyInputStream *inInputStream) override;

    void HandleWelcomePacket( MyInputStream& inInputStream );
    void HandleStatePacket( MyInputStream& inInputStream );
    void HandleGameObjectState( MyInputStream& inInputStream );


    /// send
    void SendOutgoingPackets();

    void UpdateSayingHello();
    void SendHelloPacket();

    void UpdateSendingInputPacket();
    void SendInputPacket();

private:
    MyNetworkManagerClient();
    MyReplicationManagerClient	mReplicationManagerClient;
    MySimpleTcpClient mSimpleTcpClient;

    NetworkClientState	mState;

    float				mTimeOfLastHello;
    float				mTimeOfLastInputPacket;

    std::string				mName;
    int					mPlayerId;

    float				mLastMoveProcessedByServerTimestamp;
};


#endif
