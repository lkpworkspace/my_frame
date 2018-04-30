#ifndef __MYFRAME_H__
#define __MYFRAME_H__

#include "MyCommon.h"

// data structure class
#include "MyHash.h"
#include "MyHeap.h"
#include "MyVec.h"
#include "MyList.h"

// help and log class
#include "MyHelp.h"
#include "MyLog.h"

// base class
#include "MyObj.h"
#include "MyDataParser.h"
#include "MyMath.h"
#include "MyEvent.h"
#include "MyMsgPool.h"
#include "MyIOStream.h"

// Algorithm
//#include "MyBase64.h"

// database
#include "MySqlite3.h"

// thread
#include "MyThread.h"
#include "MyTask.h"
//#include "MyAllEvent.h"

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
