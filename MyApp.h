#ifndef MYAPP_H
#define MYAPP_H
#include "Common.h"
#include <sys/epoll.h>
#include <pthread.h>
#include "MyEvent.h"
#include "MyThread.h"
#include <vector>
#include "MyNormalEvent.h"
#include "MyTimer.h"

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
    void DelLater(MyEvent* ev, int ms);
    void SetQuitFunc(common_func_t func){m_quit_func = func;}
    int Exec();                        // mainloop
    int Quit();                        // exit this app

    MyNormalEvent* GetNormalEvent(){return m_nor_event;}
private:
    int InitApp();
    void Run();                        // override Mythread method (do nothing)
    void OnInit();                     // override Mythread method (do nothing)
    void OnExit();                     // override Mythread method (do nothing)
    int CreateTask();                  // create thread
    int TimerCheck();                  // get most nearly timer
    void QuitCheck();                  // if m_isQuit, clear all resource
    void DelUselessTimer();            // delete m_useless_timer vector
    static void* DeleteTimer(void* arg);

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

    MyNormalEvent* m_nor_event;        // quit event
    bool m_isQuit;                     // quit flag
    common_func_t m_quit_func;         // quit func

    std::vector<MyTimer*> m_useless_timer;   // useless timer

    int m_cur_thread_size;             // how many task was create
    int m_cur_ev_size;                 // how many ev was create
};

} // end namespace
#endif // MYAPP_H
