#ifndef __MyLog_H__
#define __MyLog_H__

#include <mutex>
#include <string.h>
#include <errno.h>

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
        std::string __temp; \
        __temp += __FILE__; \
        __temp += ","; \
        __temp += __FUNCTION__; \
        __temp += ",line "; \
        __temp += MyHelp::ToStr(__LINE__,10); \
        __temp += ": "; \
        __temp += str; \
        MyLog::GetInstance()->AddLog(__temp); \
    }while(0)

#define __MyDebugPrint(str) do \
    { \
        std::string __temp; \
        __temp += __FILE__; \
        __temp += ","; \
        __temp += __FUNCTION__; \
        __temp += ",line "; \
        __temp += MyHelp::ToStr(__LINE__,10); \
        __temp += ": "; \
        __temp += str; \
        MyLog::GetInstance()->PrintLog(__temp); \
    }while(0)


#define MyDebug(...) do \
    { \
        char __buf[256] = {0}; \
        memset(__buf,0,sizeof(__buf)); \
        sprintf(__buf,__VA_ARGS__); \
        __MyDebug(__buf); \
    }while(0)

#define MyDebugPrint(args...) do \
    { \
        char __buf[256] = {0}; \
        memset(__buf,0,sizeof(__buf)); \
        sprintf(__buf,args); \
        __MyDebugPrint(__buf); \
    }while(0)

/*
编写此处时sprintf报错，查明原因并不是写的有错，而是调用的地方有错
*/
#define MyError(...) do \
    { \
        char* __err_str = strerror(errno); \
        int __index = 0; \
        char __buf[256] = {0}; \
        memset(__buf,0,sizeof(__buf)); \
        sprintf(__buf,"[ERROR]: "); \
        __index += 9; \
        sprintf(&__buf[__index],__VA_ARGS__); \
        __index = strlen(__buf); \
        sprintf(&__buf[__index],": %s",__err_str); \
        printf("%s",__buf); \
        __MyDebug(__buf); \
        exit(-1); \
    }while(0)

#endif // MyLog_h
