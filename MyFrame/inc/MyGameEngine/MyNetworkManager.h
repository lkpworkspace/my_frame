#ifndef MyNetworkManager_H
#define MyNetworkManager_H

typedef unordered_map< int, MyGameObj* > IntToGameObjectMap_t;

class MyNetworkManager
{
public:
    static const uint32_t	kHelloCC = 'HELO';
    static const uint32_t	kWelcomeCC = 'WLCM';
    static const uint32_t	kStateCC = 'STAT';
    static const uint32_t	kInputCC = 'INPT';


    virtual void	ProcessPacket( MyInputStream* inGameMsg) = 0;

    void ProcessIncomingPackets(MyList* inList);
    inline	MyGameObj*	GetGameObject( int inNetworkId ) const;
    void AddToNetworkIdToGameObjectMap( MyGameObj* inGameObject );
    void RemoveFromNetworkIdToGameObjectMap( MyGameObj* inGameObject );
protected:
    IntToGameObjectMap_t mNetworkIdToGameObjectMap;
private:
    void	ReadIncomingPacketsIntoQueue(MyList* inList);
    void	ProcessQueuedPackets();

    MyList mPacketQueue;
};

inline	MyGameObj* MyNetworkManager::GetGameObject( int inNetworkId ) const
{
    auto gameObjectIt = mNetworkIdToGameObjectMap.find( inNetworkId );
    if( gameObjectIt != mNetworkIdToGameObjectMap.end() )
    {
        return gameObjectIt->second;
    }
    else
    {
        return nullptr;
    }
}

#endif
