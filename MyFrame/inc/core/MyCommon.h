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

#define USING_MYFRAME       using namespace myframe
#define NS_MYFRAME_BEGIN    namespace myframe {
#define NS_MYFRAME_END      }

/**
 * 定义一些全局需要使用的函数
 */


/**
 * 获得任务名对应的任务ID
 *      一般通过该函数获得要发送消息的任务ID
 */
int GetTaskIdentify(const char* inTaskName);

/**
 * 设置任务名对应的任务ID
 */
void SetTaskIdentifyWithName(const char* inTaskName, int inTaskId);


#endif // COMMON_H
