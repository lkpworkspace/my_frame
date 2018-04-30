#include "MyServerShared.h"


int main()
{
#if 1
    /// 初始化游戏服务器资源
    if( MyGameServer::StaticInit() )
    {
        /// 启动游戏服务器
        return MyGameServer::sInstance->Go();
    }
    else
    {
        //error
        return -1;
    }
#else
//#include <unordered_map>
//    std::unordered_map<int,int> map;
//    map[10] = 10;
//    map[11] = 11;
//    map.erase(0);

////    for( const auto& pair: mPlayerIdToClientMap )
////    {
////        pair.second->GetReplicationManagerServer().ReplicateCreate( newNetworkId, inGameObject->GetAllStateMask() );
////    }
//    for(const auto& pair : map)
//    {
//        printf("some%d %d\n", pair.first, pair.second);
//    }
    MyOutputStream o;
    int state = 0x00;
    o.Write(state);
    o.Write(3,16);

    MyInputStream i(o.GetBufferPtr(),o.GetBitLength());

    int count = 0;
    i.Read(state);
    i.Read(count, 16);

    printf("count %d\n",count);


#endif
}
