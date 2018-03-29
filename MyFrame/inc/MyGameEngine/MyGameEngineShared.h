#ifndef MyGameEngineShared_H
#define MyGameEngineShared_H

#include "memory"

#include "vector"
#include "unordered_map"
#include "string"
#include "list"
#include "queue"
#include "deque"
#include "unordered_set"
#include "cassert"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::list;
using std::deque;
using std::unordered_map;
using std::string;
using std::unordered_set;

class MyGameObj;

#include "MyIOStream.h"
#include "MyGameMsg.h"

#include "MyWorld.h"
#include "MyEngine.h"

#include "MyInputState.h"
#include "MyMove.h"
#include "MyMoveList.h"

#include "MyGameObj.h"
#include "MyTiming.h"

#include "MyNetworkManager.h"
#include "MyReplicationCommand.h"

#include "MyPlane.h"

#endif
