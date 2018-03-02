#ifndef MYFRAME_H
#define MYFRAME_H

#include "MyCommon.h"

// data structure class
#include "MyHash.h"
#include "MyHeap.h"
#include "MyVec.h"
#include "MyList.h"

// base class
#include "MyLog.h"
#include "MyHelp.h"
#include "MyObj.h"
#include "MyEvent.h"
#include "MyMsgPool.h"

// Algorithm
//#include "MyBase64.h"

// database
#include "MySqlite3.h"

// thread
#include "MyThread.h"
#include "MyTask.h"
#include "MyAllEvent.h"

// network
#include "MyNet.h"
#include "MyTcp.h"
#include "MyUdp.h"

// main class
#include "MyApp.h"

#if 1
USING_MYFRAME;
#endif

#if 0 // user maybe used class
#include "MyMouseEvent.h"
#include "MyKeyEvent.h"
#include "MyTFTP.h"
#endif

#endif // MYFRAME_H
