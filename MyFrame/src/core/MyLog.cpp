#include "MyLog.h"
#include <time.h>
#include <stdio.h>

MyLog *MyLog::instance = nullptr;
char MyLog::m_szLogPath[MAX_PATH] = {0};

MyLog::MyLog(std::string logName)
{
	sprintf(m_szLogPath, "%s", logName.c_str());
	m_pLogFile = NULL;
}
MyLog::~MyLog()
{}
MyLog* MyLog::GetInstance()
{
    if(!instance)
        instance = new MyLog(LOG_NAME);
    return instance;
}

int MyLog::AddLog(std::string pszLogTxt)
{
	time_t t = time(0);
	char szTime[MAX_PATH];
	strftime(szTime, sizeof(szTime), "%Y/%m/%d %X", localtime(&t));

    m_log_mutex.lock();
	m_pLogFile = fopen(m_szLogPath, "a+");
    fprintf(m_pLogFile, "[%s]%s\n", szTime, pszLogTxt.c_str());
	fclose(m_pLogFile);
    m_log_mutex.unlock();
	return 0;
}
int MyLog::PrintLog(std::string pszLogTxt)
{
    time_t t = time(0);
    char szTime[MAX_PATH];
    strftime(szTime, sizeof(szTime), "%Y/%m/%d %X", localtime(&t));
    printf("[%s]%s", szTime, pszLogTxt.c_str());
    fflush(stdout);
    return 0;
}

void MyLog::SetLogPath(const char* path)
{
    memset(m_szLogPath,0,MAX_PATH);
    strcpy(m_szLogPath, path);
}
int MyLog::ClearLog()
{
    m_log_mutex.lock();
	m_pLogFile = fopen(m_szLogPath, "w");
	fclose(m_pLogFile);
    m_log_mutex.unlock();
	return 0;
}
