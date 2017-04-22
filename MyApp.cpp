#include "MyApp.h"
#include "MyTask.h"
#include "MyLog.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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
    MyDebugPrint("Get Epoll fd = %d\n", m_epollFd);
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
        MyDebugPrint("add event fail %d\n",res);
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
        MyDebugPrint("del event fail %d\n", res);
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
#if USE_CONFIG
    this->Start();
#endif
    while(m_cur_thread_size)
    {
        CheckStopTask();
        wait = TimerCheck();
        res = epoll_wait(m_epollFd,ev,evc,wait);
        if(res > 0)
        {
            HandleEvent(ev,res);
        }
    }

    // quit MyApp
    free(ev);
#if USE_CONFIG
    this->Stop();
#endif
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
        case MyEvent::TASKFD:
            HandleTaskEvent(event);
            break;
        case MyEvent::SOCKFD:
        case MyEvent::FILEFD:
        default:
#if DEBUG_ERROR
            MyDebugPrint("get sock,file... event\n");
#endif
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
#if DEBUG_ERROR
            printf("trans main event to task %d\n",task->GetThreadId());
#endif
            task->SendMsg(&ch,MSG_LEN);
        }else if(!m_ev_recv.IsEmpty())
        {
            // change m_idle_tasks to m_ev_recv
            task->m_que.Append(&m_ev_recv);
#if DEBUG_ERROR
            printf("trans self event to task self %d\n",task->GetThreadId());
#endif
            task->SendMsg(&ch,MSG_LEN);      
        }else
        {
            m_idle_tasks.AddTail(task);
#if DEBUG_ERROR
            printf("add task %d to idle queue\n",task->GetThreadId());
#endif
        }
    }
}
////////////////////////////////////////////////////
/// thread virtual method (use msg config our server)
#if USE_CONFIG
typedef struct my_config_t
{
#if 0
    mqd_t msg_queue;
    std::string msg_queue_name;
    char buf[10];
    my_config_t()
    {
        msg_queue = 0;
        msg_queue_name = "my_frame_msg_queue";
        memset(buf,0,sizeof(buf));
    }
#else
    char buf[10];
    key_t key_id;
    typedef struct msgstru_t
    {
        long msgtype;
        char msgtext[1024];
    }msgstru_t;
    msgstru_t msgstru;
#endif
}my_config_t;
static my_config_t g_config;
#endif
void MyApp::OnInit()
{
#if USE_CONFIG
#if 0
    g_config.msg_queue = mq_open(" ",O_RDWR | O_CREAT);
    perror("mq_open");
    assert(g_config.msg_queue != -1);
#else // use system V
    g_config.key_id = msgget(MSG_KEY,IPC_CREAT);
    assert(g_config.key_id != -1);
#endif
#endif
}
void MyApp::Run()
{
#if USE_CONFIG
#if 0
    if(!mq_receive(g_config.msg_queue, g_config.buf, sizeof(g_config.buf),NULL))
    {
        printf("%s\n",g_config.buf);
        memset(g_config.buf,0,sizeof(g_config.buf));
    }
#else
    int res = msgrcv(g_config.key_id,(void*)&g_config.msgstru,(size_t)sizeof(g_config.msgstru),0,0);
    perror("msgrcv");
    assert(res != -1);
    // TODO...

    printf("%s\n",g_config.msgstru.msgtext);
#endif
#endif
}
void MyApp::OnExit()
{
#if USE_CONFIG
#if 0
    if(g_config.msg_queue != 0)
        mq_close(g_config.msg_queue);
    mq_unlink(g_config.msg_queue_name.c_str());
#else
    msgctl(g_config.key_id,IPC_RMID,0);
#endif
#endif
}
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






















