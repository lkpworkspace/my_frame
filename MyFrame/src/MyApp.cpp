#include "../inc/MyApp.h"
#include "../inc/MyTask.h"
#include "../inc/MyLog.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace my_master;
MyApp* MyApp::theApp = nullptr;

MyApp::MyApp(int thread_size)
{
    theApp = this;
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
    m_isQuit = false;
    m_quit_func = nullptr;
    // epoll create
    m_epollFd = epoll_create1(0);
    if(m_epollFd == -1)
        MyError("epoll create");
#ifdef USE_LOG
    MyDebug("MyApp Begin");
    MyDebug("Get Epoll fd = %d", m_epollFd);
#endif
#if DEBUG_INFO
    MyDebugPrint("MyApp Begin\n");
    MyDebugPrint("Get Epoll fd = %d\n", m_epollFd);
#endif
    // task create
    for(int i = 0; i < m_threadSize; ++i)
    {
        CreateTask();
    }
    // init normal event
    m_nor_event = new MyNormalEvent;

    // ingore SIGPIPE signal
    signal(SIGPIPE,SIG_IGN);
    return 0;
}
int MyApp::CreateTask()
{
    MyTask* task = new MyTask;
    this->AddEvent(task);

    pthread_mutex_lock(&m_app_mutex);
    ++m_cur_thread_size;
    pthread_mutex_unlock(&m_app_mutex);
    return 0;
}
int MyApp::AddEvent(MyEvent* ev)
{
    struct epoll_event event;
    int res;

    pthread_mutex_lock(&m_app_mutex);
    event.data.ptr = ev;
    event.events = ev->GetEpollEventType();
    if(epoll_ctl(m_epollFd,EPOLL_CTL_MOD,ev->GetEventFd(),&event) < 0)
    {
        res = epoll_ctl(m_epollFd,EPOLL_CTL_ADD,ev->GetEventFd(),&event);
        if(res)
        {
            #if DEBUG_INFO
            MyDebugPrint("add event fail %d\n",res);
            #endif
            #ifdef USE_LOG
            MyError("add event fail %d",res);
            #endif
        }
    }else
    {
        #if DEBUG_INFO
        MyDebugPrint("epoll_ctl_mod sucess\n");
        #endif
        #ifdef USE_LOG
        MyError("epoll_ctl_mod sucess\n");
        #endif
    }

    ++m_cur_ev_size;
    pthread_mutex_unlock(&m_app_mutex);
    return res;
}
int MyApp::DelEvent(MyEvent* ev)
{
    pthread_mutex_lock(&m_app_mutex);
    int res = epoll_ctl(m_epollFd,EPOLL_CTL_DEL,ev->GetEventFd(),NULL);
    if(res)
    {
        #if DEBUG_INFO
            MyDebugPrint("del event fail %d\n", res);
        #endif
        #ifdef USE_LOG
            MyError("del event fail %d", res);
        #endif
    }

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
        QuitCheck();
        CheckStopTask();
        DelUselessTimer();
        wait = TimerCheck();
        res = epoll_wait(m_epollFd,ev,evc,wait);
        if(res > 0)
        {
            HandleEvent(ev,res);
        }
        // 也许是中断
//        else if(res == -1)
//        {
//            MyDebugPrint("epoll wait error\n");
//            MyError("epoll wait");
//        }
    }

    // quit MyApp
    if(m_nor_event)
        delete m_nor_event;
    if(m_quit_func)
        m_quit_func(NULL);
    free(ev);
    close(m_epollFd);
#if USE_CONFIG
    this->Stop();
#endif
#ifdef USE_LOG
    MyDebug("MyApp Quit");
#endif
#if DEBUG_INFO
    MyDebugPrint("MyApp Quit\n");
#endif
    return 0;
}

int MyApp::Quit()
{
    // set quit flag
    pthread_mutex_lock(&m_app_mutex);
    m_isQuit = true;
    pthread_mutex_unlock(&m_app_mutex);
    // weakup mainloop to check quit flag
    m_nor_event->Work();
    return 0;
}

/////////////////////////////////////////////////////
int MyApp::TimerCheck()
{
    // return  millisecond
    return MyTimer::TimerCheck();
}
void MyApp::QuitCheck()
{
    pthread_mutex_lock(&m_app_mutex);
    if(m_isQuit)
    {
        MyNode* temp;
        MyNode* begin = m_idle_tasks.Begin();
        MyNode* end = m_idle_tasks.End();
        for(;begin != end;)
        {
            temp = begin->next;
            MyTask *task = (MyTask*)begin;
            task->Quit();
            m_idle_tasks.Del(begin);
            begin = temp;
        }
    }
    pthread_mutex_unlock(&m_app_mutex);
    return;
}

void MyApp::CheckStopTask()
{
    char ch = 'y';

    MyTask* begin;
    MyTask* end;
    MyTask* temp;

    begin= (MyTask*)m_idle_tasks.Begin();
    end = (MyTask*)m_idle_tasks.End();
    for(;begin != end;)
    {
        temp = (MyTask*)(begin->next);
        // move MyTask recv queue to MyTask work queue
        // delete from idle task queue
        // weakup this task, continue
        if(!begin->m_recv.IsEmpty())
        {
            begin->m_que.Append(&begin->m_recv);
            // be careful with this operator
            m_idle_tasks.Del((MyNode*)begin,false);
            begin->SendMsg(&ch,MSG_LEN);
            begin = (MyTask*)(temp);
            continue;
        }
        // move MyApp recv queue to MyTask work queue
        // weak up this task
        if(!m_ev_recv.IsEmpty())
        {
            begin->m_que.Append(&m_ev_recv);
            // be careful with this operator
            m_idle_tasks.Del((MyNode*)begin,false);
            begin->SendMsg(&ch,MSG_LEN);
            begin = (MyTask*)(temp);
            continue;
        }
        begin = (MyTask*)(temp);
    }
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
        MyDebugPrint("main thread : get task %d quit msg\n",task->GetThreadId());
        delete task;
        m_cur_thread_size--;
    }else
    {
        // task need event
        if(!task->m_recv.IsEmpty())
        {
            task->m_que.Append(&(task->m_recv));
#if DEBUG_ERROR
            MyDebugPrint("trans main event to task %d\n",task->GetThreadId());
#endif
            task->SendMsg(&ch,MSG_LEN);
        }else if(!m_ev_recv.IsEmpty())
        {
            // change m_idle_tasks to m_ev_recv
            task->m_que.Append(&m_ev_recv);
#if DEBUG_ERROR
            MyDebugPrint("trans self event to task self %d\n",task->GetThreadId());
#endif
            task->SendMsg(&ch,MSG_LEN);
        }else
        {
            m_idle_tasks.AddTail(task);
#if DEBUG_ERROR
            MyDebugPrint("add task %d to idle queue\n",task->GetThreadId());
#endif
        }
    }
}

void MyApp::DelUselessTimer()
{
    if(!m_useless_timer.empty())
    {
        //delete all useless timer
        std::vector<MyTimer*>::iterator iter;
        for(iter = m_useless_timer.begin(); iter != m_useless_timer.end();)
        {
            delete *iter;
            iter = m_useless_timer.erase(iter);
        }
    }
}

void MyApp::DelLater(MyEvent* ev, int ms)
{
    MyTimer* timer = new MyTimer(ms);

    void** arg = new void*[2];
    arg[0] = timer;
    arg[1] = ev;

    timer->SetCallFunc(&MyApp::DeleteTimer,(void*)arg);
    timer->Start();
}

void* MyApp::DeleteTimer(void* arg)
{
    void** a = (void**)arg;
    MyTimer* timer = (MyTimer*)a[0];
    MyEvent* ev = (MyEvent*)a[1];

    if(ev)
    {
        delete (MyEvent*)ev;
        ev = NULL;
    }
    timer->Stop();
    MyApp::theApp->m_useless_timer.push_back(timer);
    delete[] a;
    return NULL;
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






















