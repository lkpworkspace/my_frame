#ifndef MYAPP_H
#define MYAPP_H
#include "Common.h"
#include <sys/epoll.h>
#include <pthread.h>
#include "MyEvent.h"
#include "MyThread.h"
namespace my_master {
class MyTask;
class MyApp : public MyThread
{
public:
    static MyApp* theApp;
    MyApp(int thread_size = 1,int ev_size = 1024);
    ~MyApp();

    int AddEvent(MyEvent* ev);
    int DelEvent(MyEvent* ev);
    int Exec();                        // mainloop
private:
    int InitApp();
    void Run();                        // override Mythread method (do nothing)
    void OnInit();                     // override Mythread method (do nothing)
    void OnExit();                     // override Mythread method (do nothing)
    int CreateTask();                  // create thread
    int TimerCheck();                  // get most nearly timer

    void CheckStopTask();
    void HandleEvent(struct epoll_event* epev, int count);
    void HandleTaskEvent(MyEvent*);
private:
    std::vector<MyTask*> m_tasks;      // save MyTask class(not use)
    my_master::MyList m_idle_tasks;    // save MyTask class
    my_master::MyList m_ev_recv;       // recv task event, save MyEvent class
    pthread_mutex_t m_app_mutex;
    int m_epollFd;                     // listen Event file des
    int m_evSize;                      // can be listened (const var)
    int m_threadSize;                  // thread size (const var)

    int m_cur_thread_size;             // how many task was create
    int m_cur_ev_size;                 // how many ev was create
};

} // end namespace
#endif // MYAPP_H
