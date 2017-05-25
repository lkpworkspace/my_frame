#include "MyTask.h"
#include "MyApp.h"
#include "MyLog.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
using namespace my_master;
MyTask::MyTask()
{
    m_msgFd[0] = -1;
    m_msgFd[1] = -1;
    memset(m_msgBuf,0,MSG_LEN);
    m_isQuit = true;
    CreateSockPair();
    this->Start();
}
MyTask::~MyTask()
{
    ClearResource();
#if DEBUG_ERROR
    MyDebugPrint("MyTask %d destructor\n", GetThreadId());
#endif
}
////////////////////////////////////////////////////Runing thread
void MyTask::Run()
{
    if(m_isQuit && m_que.IsEmpty())
    {
        this->Stop();
        return;
    }

    WaitEvent();
    TaskWork();
}
int MyTask::TaskWork()
{
#if DEBUG_ERROR
    MyDebugPrint("task %d work, has %d event\n",GetThreadId(),m_que.Count());
#endif
    MyEvent* temp;
    MyEvent* begin = (MyEvent*)m_que.Begin();
    MyEvent* end = (MyEvent*)m_que.End();
    while(begin != end)
    {
#if 0
        static void* b = begin;
        if(begin != b)
        {
            printf("begin %p m_recv %p\n",b,m_recv.End());
            //assert(false);
        }
#endif
        temp = (MyEvent*)(begin->next);
        begin->CallBackFunc(begin);
        m_que.Del((MyNode*)begin,false);
        begin = (MyEvent*)(temp);
    }
    return 0;
}
////////////////////////////////////////////////////TODO...
int MyTask::WaitEvent()
{
    // send to MyApp, this task idle
    m_msgBuf[0] = 0x01;
    write(m_msgFd[0],m_msgBuf,MSG_LEN);
#if DEBUG_ERROR
    MyDebugPrint("task %d waiting...\n",GetThreadId());
#endif
    // wait MyApp trans event
    return read(m_msgFd[0],m_msgBuf,MSG_LEN);
}
void MyTask::OnInit()
{
    MyThread::OnInit();
    m_isQuit = false;
#ifdef USE_LOG
    MyDebug("thread %u begin", GetThreadId());
#endif
#if DEBUG_INFO
    MyDebugPrint("thread %u begin\n", GetThreadId());
#endif
}
void MyTask::OnExit()
{
    MyThread::OnExit();
    m_isQuit = true;
#ifdef USE_LOG
    MyDebug("thread %u quit", GetThreadId());
#endif
#if DEBUG_INFO
    MyDebugPrint("thread %u quit\n", GetThreadId());
#endif
    // send stop msg to main thread
    char ch = -1;
    write(m_msgFd[0],&ch,MSG_LEN);
}
int MyTask::CreateSockPair()
{
    int res = -1;
    res = socketpair(AF_UNIX,SOCK_DGRAM,0,m_msgFd);
    assert(res == 0);

    Common::SetNonblock(m_msgFd[0],false);
    Common::SetNonblock(m_msgFd[1],false);
    return res;
}
////////////////////////////////////////////////////
/// MyApp communication with this class
int MyTask::SendMsg(const char* buf, int len)
{
    return write(m_msgFd[1],buf,len);
}
int MyTask::RecvMsg(char* buf, int len)
{
    return read(m_msgFd[1],buf,len);
}
void MyTask::Quit()
{
    Lock();
    m_isQuit = true;
    Unlock();
    // weakup this task
    char ch = -1;
    SendMsg(&ch,1);
}
////////////////////////////////////////////////////
/// assist function
void MyTask::ClearResource()
{
    this->Stop();
    close(m_msgFd[0]);
    close(m_msgFd[1]);
}







































