#ifndef MyServerShared_H
#define MyServerShared_H

#if 1
#define IP "127.0.0.1"
#else
#define IP "172.16.212.139"
#endif
#define PORT 4399


#include "MyGameEngineShared.h"


#include "Server/MyReplicationManagerServer.h"

#include "Server/MyClientProxy.h"
#include "Server/MyNetworkManagerServer.h"
#include "Server/MyServer.h"

#include "Server/MyPlaneServer.h"

#endif
