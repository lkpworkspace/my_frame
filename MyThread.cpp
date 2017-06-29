#include "MyThread.h"
#include "Common.h"
#include "MyLog.h"
using namespace my_master;
MyThread::MyThread()
{
    m_thread = -1;
    m_id = -1;
    m_isRuning = false;
    pthread_mutex_init(&m_mutex,NULL);
}

MyThread::~MyThread()
{
    Stop();
    pthread_mutex_destroy(&m_mutex);
}

void MyThread::Start()
{
    Lock();
    int res = 0;
    if(!m_isRuning)
    {
        m_isRuning = true;
        res = pthread_create(&m_thread,NULL,&MyThread::ListenThread,this);
        if(res != 0)
        {
            MyDebugPrint("pthread create fail\n");
            MyError("pthread_create");
        }
        res = pthread_detach(m_thread);
        if(res != 0)
        {
            MyDebugPrint("pthread detach fail\n");
            MyError("pthread_detach");
        }
    }
    Unlock();
}

void MyThread::Stop()
{
    Lock();
    m_isRuning = false;
#if 0
    pthread_cancel(m_thread);
#endif
    Unlock();
}

void MyThread::Lock()
{
    pthread_mutex_lock(&m_mutex);
}

void MyThread::Unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

bool MyThread::CheckStatus()
{
    Lock();
    bool is_run = m_isRuning;
    Unlock();
    return is_run;
}

void* MyThread::ListenThread(void* obj)
{
	MyThread* t = (MyThread*)obj;
    t->OnInit();
    while (1)
	{
        if(!t->CheckStatus())
            break;
		t->Run();
	}
	t->OnExit();
    return NULL;
}
