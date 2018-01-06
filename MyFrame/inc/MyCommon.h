#ifndef __COMMON_H__
#define __COMMON_H__
// c normal
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
// c++ normal
#include <iostream>
#include <string>
// win or linux
#ifdef WIN32
#include <Windows.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <unistd.h>
#include "MyHelp.h"
#endif
// log define
#define USE_LOG
// debug define
#ifndef DEBUG_INFO
#define DEBUG_INFO 1
#endif
#ifndef DEBUG_WARNING
#define DEBUG_WARNING 0
#endif
#ifndef DEBUG_ERROR
#define DEBUG_ERROR 0
#endif
// system V (msg key id)
#define MSG_KEY 1024
#define MSG_LEN 1
#define USE_CONFIG 0
// unuse arg
#define UNUSE_ARG(argc,argv) \
    do{ \
        argc = argc; \
        argv = argv; \
    }while(0)
#define UNUSE(arg) (arg) = (arg)


#endif // COMMON_H