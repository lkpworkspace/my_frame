#ifndef MyReplicationManagerClient_H
#define MyReplicationManagerClient_H

class MyReplicationManagerClient
{
public:
    void Read( MyInputStream& inInputStream );

private:

    void ReadAndDoCreateAction( MyInputStream& inInputStream, int inNetworkId );
    void ReadAndDoUpdateAction( MyInputStream& inInputStream, int inNetworkId );
    void ReadAndDoDestroyAction( MyInputStream& inInputStream, int inNetworkId );

};

#endif
