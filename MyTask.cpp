#include "MyTask.h"
#include "MyApp.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
using namespace my_master;
MyTask::MyTask()
{
    m_msgFd[0] = -1;
    m_msgFd[1] = -1;
    memset(m_msgBuf,0,MSG_LEN);
    //sem_init(&m_event,0,0);
    CreateSockPair();
    this->Start();
}
MyTask::~MyTask(){ClearResource();}
////////////////////////////////////////////////////Runing thread
void MyTask::Run()
{
    WaitEvent();
    TaskWork();
}
int MyTask::TaskWork()
{
#if 1
    printf("task %d work, has %d event\n",GetThreadId(),m_que.Count());
#endif
    MyEvent* begin = (MyEvent*)m_que.Begin();
    MyEvent* end = (MyEvent*)m_que.End();
    while(begin != end)
    {
        begin->CallBackFunc(begin);
        m_que.Del((MyNode*)begin,false);
        begin = (MyEvent*)(begin->next);
    }
    return 0;
}
////////////////////////////////////////////////////TODO...
int MyTask::WaitEvent()
{
    // send to MyApp, this task idle
    m_msgBuf[0] = 0x01;
    write(m_msgFd[0],m_msgBuf,MSG_LEN);
#if 1
    printf("task %d waiting...\n",GetThreadId());
#endif
    // wait MyApp trans event
    return read(m_msgFd[0],m_msgBuf,MSG_LEN);
}
void MyTask::OnInit()
{
    MyThread::OnInit();
#ifdef USE_LOG
    MyDebug("thread %u begin", GetThreadId());
#endif
#if DEBUG_INFO
    printf("thread %u begin\n", GetThreadId());
#endif
}
void MyTask::OnExit()
{
    MyThread::OnExit();
#ifdef USE_LOG
    MyDebug("thread %u quit", GetThreadId());
#endif
#if DEBUG_INFO
    printf("thread %u quit\n", GetThreadId());
#endif
}
int MyTask::CreateSockPair()
{
    int res = -1;
    res = socketpair(AF_UNIX,SOCK_DGRAM,0,m_msgFd);
#ifdef USE_LOG
    MyDebug("create socketpair %d",res);
#endif
#if DEBUG_INFO
    printf("create socketpair %d\n",res);
#endif
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
////////////////////////////////////////////////////
/// assist function
void MyTask::ClearResource()
{
    this->Stop();
    close(m_msgFd[0]);
    close(m_msgFd[1]);
    //sem_destroy(&m_event);
}







































