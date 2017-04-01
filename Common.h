#ifndef __COMMON_H__
#define __COMMON_H__
// c normal
#include <stdio.h>
// c++ normal
#include <iostream>
#include <cstdio>
#include <string>
#include <cassert>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <cstring>
// c++ stl
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
// win or linux
#ifdef WIN32
#include <Windows.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <unistd.h>
#endif
// my class
#include "MyLog.h"
#include "MyHelp.h"
#include "MyVec.h"
#include "MyList.h"
#include "MyEvent.h"
#include "MyThread.h"

#define USE_LOG

#ifndef DEBUG_INFO
#define DEBUG_INFO 1
#endif

#ifndef DEBUG_WARNING
#define DEBUG_WARNING 0
#endif

#ifndef DEBUG_ERROR
#define DEBUG_ERROR 1
#endif

class Common
{
public:
    static bool SetNonblock(int fd, bool b);
    static int GetFileLen(const char* path);
    static bool IsFileExist(const char* path);
    static bool IsDirExist(const char* path);
    static char* GetHomeDir();
    static uint64_t GetTimerNow();
private:
    Common();
    ~Common();
};

#endif // COMMON_H
