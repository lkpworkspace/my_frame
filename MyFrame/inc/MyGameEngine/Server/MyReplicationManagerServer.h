#ifndef MyReplicationManagerServer_H
#define MyReplicationManagerServer_H

class MyReplicationManagerServer
{

public:
    void ReplicateCreate( int inNetworkId, uint32_t inInitialDirtyState );
    void ReplicateDestroy( int inNetworkId );
    void SetStateDirty( int inNetworkId, uint32_t inDirtyState );
    void RemoveFromReplication( int inNetworkId );

    void Write( MyOutputStream& inOutputStream );

private:

    uint32_t WriteCreateAction( MyOutputStream& inOutputStream, int inNetworkId, uint32_t inDirtyState );
    uint32_t WriteUpdateAction( MyOutputStream& inOutputStream, int inNetworkId, uint32_t inDirtyState );
    uint32_t WriteDestroyAction( MyOutputStream& inOutputStream, int inNetworkId, uint32_t inDirtyState );

    unordered_map< int, MyReplicationCommand >	mNetworkIdToReplicationCommand;
    vector< int >								mNetworkIdsToRemove;


};

#endif
