#ifndef __MyLog_H__
#define __MyLog_H__
#include <mutex>
#include <string.h>
#include <errno.h>
#include "MyHelp.h"
namespace my_master {

#define LOG_NAME "myframe.log"
#define MAX_PATH 255

class MyLog
{
public:
    static MyLog* GetInstance();
    static void SetLogPath(const char* path); // do not suggest to use
    int AddLog(std::string pszLogTxt);
    int PrintLog(std::string pszLogTxt);
	int ClearLog();
protected:
    static char m_szLogPath[MAX_PATH];    // file path
    FILE* m_pLogFile;                     // file pointer
    std::mutex m_log_mutex;               // mutex
private:
    MyLog(std::string logName);
    ~MyLog();
private:
    static MyLog* instance;
};

#define __MyDebug(str) do \
    { \
        std::string temp; \
        temp += __FILE__; \
        temp += ","; \
        temp += __FUNCTION__; \
        temp += ",line "; \
        temp += MyHelp::ToStr(__LINE__,10); \
        temp += ": "; \
        temp += str; \
        my_master::MyLog::GetInstance()->AddLog(temp); \
    }while(0)

#define __MyDebugPrint(str) do \
    { \
        std::string temp; \
        temp += __FILE__; \
        temp += ","; \
        temp += __FUNCTION__; \
        temp += ",line "; \
        temp += MyHelp::ToStr(__LINE__,10); \
        temp += ": "; \
        temp += str; \
        my_master::MyLog::GetInstance()->PrintLog(temp); \
    }while(0)


#define MyDebug(...) do \
    { \
        char buf[2048] = {0}; \
        memset(buf,0,sizeof(buf)); \
        sprintf(buf,__VA_ARGS__); \
        __MyDebug(buf); \
    }while(0)

#define MyDebugPrint(args...) do \
    { \
        char buf[2048] = {0}; \
        memset(buf,0,sizeof(buf)); \
        sprintf(buf,args); \
        __MyDebugPrint(buf); \
    }while(0)

/*
编写此处时sprintf报错，查明原因并不是写的有错，而是调用的地方有错
*/
#define MyError(...) do \
    { \
        char* err_str = strerror(errno); \
        int index = 0; \
        char buf[2048] = {0}; \
        memset(buf,0,sizeof(buf)); \
        sprintf(buf,"[ERROR]: "); \
        index += 9; \
        sprintf(&buf[index],__VA_ARGS__); \
        index = strlen(buf); \
        sprintf(&buf[index],": %s",err_str); \
        printf("%s",buf); \
        __MyDebug(buf); \
        exit(-1); \
    }while(0)

} // end namespace
#endif // MyLog_h
