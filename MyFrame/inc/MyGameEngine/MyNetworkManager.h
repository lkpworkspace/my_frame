#ifndef MyNetworkManager_H
#define MyNetworkManager_H

class MyNetworkManager
{
public:
    static const uint32_t	kHelloCC = 'HELO';
    static const uint32_t	kWelcomeCC = 'WLCM';
    static const uint32_t	kStateCC = 'STAT';
    static const uint32_t	kInputCC = 'INPT';


    virtual void	ProcessPacket( MyInputStream* inGameMsg) = 0;

    void	ProcessIncomingPackets(MyList* inList);

protected:
private:
    void	ReadIncomingPacketsIntoQueue(MyList* inList);
    void	ProcessQueuedPackets();

    MyList mPacketQueue;
};

#endif
