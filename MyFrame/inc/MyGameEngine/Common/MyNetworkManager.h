#ifndef MyNetworkManager_H
#define MyNetworkManager_H

typedef unordered_map< int, MyGameObj* > IntToGameObjectMap_t;

/**
 * @brief The MyNetworkManager class
 *      解包，发包
 *      索引网络游戏对象
 *
 */
class MyNetworkManager
{
public:
    /**
     *  可以接收或者发送的数据包类型
     *      接收：
     *          kHelloCC
     *          kInputCC
     *      发送：
     *          kWelcomeCC
     *          kStateCC
     */
    static const uint32_t	kHelloCC = 'HELO';
    static const uint32_t	kWelcomeCC = 'WLCM';

    static const uint32_t	kInputCC = 'INPT';
    static const uint32_t	kStateCC = 'STAT';


    /* 从客户代理收取消息，并缓存到该类 */
    void ProcessIncomingPackets(MyList* inList);

    /* (need override)处理收到的每一个消息，一般由子类去具体实现 */
    virtual void	ProcessPacket( MyInputStream* inGameMsg) = 0;

    /* 获得游戏对象 */
    inline	MyGameObj*	GetGameObject( int inNetworkId ) const;

    /* 增加/删除 游戏对象 */
    void AddToNetworkIdToGameObjectMap( MyGameObj* inGameObject );
    void RemoveFromNetworkIdToGameObjectMap( MyGameObj* inGameObject );
protected:
    IntToGameObjectMap_t mNetworkIdToGameObjectMap;
private:
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
