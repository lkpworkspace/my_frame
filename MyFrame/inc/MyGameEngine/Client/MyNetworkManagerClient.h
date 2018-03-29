#ifndef MyNetworkManagerClient_H
#define MyNetworkManagerClient_H


class NetworkManagerClient : public MyNetworkManager
{
    enum NetworkClientState
    {
        NCS_Uninitialized,
        NCS_SayingHello,
        NCS_Welcomed
    };

public:
    static NetworkManagerClient*	sInstance;
};


#endif
