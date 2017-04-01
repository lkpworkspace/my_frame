#include "MyApp.h"
#include "MyTask.h"

using namespace my_master;
MyApp* MyApp::theApp = nullptr;

MyApp::MyApp(int thread_size, int ev_size)
{
    theApp = this;
    m_evSize = ev_size;
    m_threadSize = thread_size;
    m_cur_thread_size = 0;
    m_cur_ev_size = 0;
    InitApp();
}
MyApp::~MyApp()
{}
int MyApp::InitApp()
{
    pthread_mutex_init(&m_app_mutex,NULL);
    // epoll create
    m_epollFd = epoll_create(m_evSize);
#ifdef USE_LOG
    MyDebug("Get Epoll fd = %d", m_epollFd);
#endif
#if DEBUG_INFO
    printf("Get Epoll fd = %d\n", m_epollFd);
#endif
    // task create
    for(int i = 0; i < m_threadSize; ++i)
    {
        CreateTask();
    }
    return 0;
}
int MyApp::CreateTask()
{
    // TODO...
    MyTask* task = new MyTask;
    //m_idle_tasks.AddTail(task);
    m_tasks.push_back(task);
    this->AddEvent(task);

    pthread_mutex_lock(&m_app_mutex);
    ++m_cur_thread_size;
    pthread_mutex_unlock(&m_app_mutex);
    return 0;
}
int MyApp::AddEvent(MyEvent* ev)
{
    // TODO...
    struct epoll_event event;
    int res;

    pthread_mutex_lock(&m_app_mutex);
    event.data.ptr = ev;
    event.events = ev->GetEpollEventType();
    if(epoll_ctl(m_epollFd,EPOLL_CTL_MOD,ev->GetEventFd(),&event) < 0)
        res = epoll_ctl(m_epollFd,EPOLL_CTL_ADD,ev->GetEventFd(),&event);
#ifdef USE_LOG
    if(res)
        MyDebug("add event fail %d",res);
#endif
#if DEBUG_INFO
    if(res)
        printf("add event fail %d\n",res);
#endif
    ++m_cur_ev_size;
    pthread_mutex_unlock(&m_app_mutex);
    return res;
}
int MyApp::DelEvent(MyEvent* ev)
{
    pthread_mutex_lock(&m_app_mutex);
    int res = epoll_ctl(m_epollFd,EPOLL_CTL_DEL,ev->GetEventFd(),NULL);
#ifdef USE_LOG
    if(res)
        MyDebug("del event fail %d", res);
#endif
#if DEBUG_INFO
    if(res)
        printf("del event fail %d\n", res);
#endif
    --m_cur_ev_size;
    pthread_mutex_unlock(&m_app_mutex);
    return res;
}
int MyApp::Exec()
{
    int res = 0;
    int evc = 10;
    int wait = 0;
    struct epoll_event* ev = (struct epoll_event*)malloc(sizeof(struct epoll_event) * evc);

    while(m_cur_thread_size)
    {
        CheckStopTask();
        wait = TimerCheck();
        res = epoll_wait(m_epollFd,ev,evc,wait);
        if(res > 0)
        {
#if 1
            printf("Exec get %d event\n",res);
#endif
            HandleEvent(ev,res);
        }
    }

    // quit MyApp
    free(ev);
    return 0;
}
/////////////////////////////////////////////////////
int MyApp::TimerCheck()
{
    // TODO...
    return 1000*60;
}
void MyApp::CheckStopTask()
{
    char ch = 'y';
#if 1
    MyTask* begin;
    MyTask* end;

    begin= (MyTask*)m_idle_tasks.Begin();
    end = (MyTask*)m_idle_tasks.End();
    for(;begin != end;)
    {
        // move MyTask recv queue to MyTask work queue
        // delete from idle task queue
        // weakup this task, continue
        // TODO...
        if(!begin->m_recv.IsEmpty())
        {
            begin->m_que.Append(&begin->m_recv);
            // be careful with this operator
            m_idle_tasks.Del((MyNode*)begin,false);
            begin->SendMsg(&ch,MSG_LEN);
            begin = (MyTask*)(begin->next);
            continue;
        }
        // move MyApp recv queue to MyTask work queue
        // weak up this task
        // TODO...
        if(!m_ev_recv.IsEmpty())
        {
            begin->m_que.Append(&m_ev_recv);
            m_idle_tasks.Del((MyNode*)begin,false);
            // be careful with this operator
            begin->SendMsg(&ch,MSG_LEN);
        }
        begin = (MyTask*)(begin->next);
    }
#else
    MyNode* begin;
    MyNode* end;

    begin= m_idle_tasks.Begin();
    end = m_idle_tasks.End();
    printf("end pointer %p\n",end);
    for(;begin != end;)
    {
        // move MyTask recv queue to MyTask work queue
        // delete from idle task queue
        // weakup this task, continue
        // TODO...
        if(!((MyTask*)begin)->m_recv.IsEmpty())
        {
            ((MyTask*)begin)->m_que.Append(&((MyTask*)begin)->m_recv);
            // be careful with this operator
            m_idle_tasks.Del(begin,false);
            ((MyTask*)begin)->SendMsg(&ch,MSG_LEN);
            begin = begin->next;
            continue;
        }
        // move MyApp recv queue to MyTask work queue
        // weak up this task
        // TODO...
        if(!m_ev_recv.IsEmpty())
        {
            ((MyTask*)begin)->m_que.Append(&m_ev_recv);
            m_idle_tasks.Del(begin,false);
            // be careful with this operator
            ((MyTask*)begin)->SendMsg(&ch,MSG_LEN);
        }
        printf("begin pointer %p\n",begin);
        begin = begin->next;
        printf("begin pointer %p\n",begin);
    }
#endif
}
void MyApp::HandleEvent(struct epoll_event* epev, int count)
{
    MyEvent* event = nullptr;

    for(int i = 0; i < count; ++i)
    {
        event = (MyEvent*)epev[i].data.ptr;
        switch(event->GetEventType())
        {
        case MyEvent::TASK:
            HandleTaskEvent(event);
            break;
        case MyEvent::SOCK:
        case MyEvent::FILE:
        default:
            this->DelEvent(event);
            m_ev_recv.AddTail(event);
            break;
        }
    }
}
void MyApp::HandleTaskEvent(MyEvent* ev)
{
    char ch = 'y';
    MyTask* task = (MyTask*)ev;
    // TODO...
    task->RecvMsg(&ch,MSG_LEN);
    if(ch < 0)
    {
        // thread stop
        // TODO... nothing
    }else
    {
        // task need event
        if(!task->m_recv.IsEmpty())
        {
            task->m_que.Append(&(task->m_recv));
            task->SendMsg(&ch,MSG_LEN);
        }else if(!m_ev_recv.IsEmpty())
        {
            // change m_idle_tasks to m_ev_recv
            task->m_que.Append(&m_ev_recv);
            task->SendMsg(&ch,MSG_LEN);
        }else
        {
            m_idle_tasks.AddTail(task);
        }
    }
}
////////////////////////////////////////////////////
/// thread virtual method (do nothing)
void MyApp::Run()
{}
void MyApp::OnInit()
{}
void MyApp::OnExit()
{}
////////////////////////////////////////////////////// test func
void TestRun()
{
#if 0
    char ch;
    while((ch = getchar()) != 'q')
    {
        printf("send msg\n");
        for(int i = 0; i < m_idle_tasks.Count(); ++i)
        {
            MyTask* task = (MyTask*)m_idle_tasks.GetData(i);
            task->SendMsg((uint8_t*)&ch,1);
        }
    }
#endif
    char ch = std::getchar();
    switch(ch)
    {
    case 'q':
        break;
    case 's':

        break;
    case 'b':
        break;
    default:
        break;
    }
}






















