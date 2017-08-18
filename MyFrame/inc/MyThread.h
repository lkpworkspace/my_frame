#ifndef __MyThread_H__
#define __MyThread_H__
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
namespace my_master {

class MyThread
{
public:
    MyThread();
    virtual ~MyThread();
    virtual void Run() = 0;
    virtual void OnInit(){m_id = syscall(SYS_gettid);}
    virtual void OnExit(){}
    void Start();
    void Stop();

    pid_t GetThreadId() {return m_id;}
    pthread_t GetPosixThreadId(){return m_thread;}
protected:
    void Lock();
    void Unlock();
    static void* ListenThread(void*);
    pthread_t m_thread;                  // posix thread id
    pid_t m_id;                          // thread id
    pthread_mutex_t m_mutex;
private:
    bool CheckStatus();
    bool m_isRuning;
};

} // end namespace
#endif
