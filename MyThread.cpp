#include "MyThread.h"
#include "Common.h"
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
    if(!m_isRuning)
    {
        m_isRuning = true;
        pthread_create(&m_thread,NULL,&MyThread::ListenThread,this);
        pthread_detach(m_thread);
    }
    Unlock();
}

void MyThread::Stop()
{
    Lock();
    m_isRuning = false;
    pthread_cancel(m_thread);
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

void* MyThread::ListenThread(void* obj)
{
	MyThread* t = (MyThread*)obj;
    t->OnInit();
    while (t->m_isRuning)
	{
		t->Run();
	}
	t->OnExit();
    return NULL;
}
