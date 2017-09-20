#ifndef MYTIMER_H
#define MYTIMER_H
#include "MyCommon.h"
#include "MyLog.h"
#include "MyNormalEvent.h"
#include <map>
#include <mutex>

class MyTimer : public my_master::MyNormalEvent
{
    friend class MyApp;
    typedef std::multimap<uint64_t, MyTimer*>::iterator timer_iter;
public:
    MyTimer(int period);
    virtual ~MyTimer();

    void SetCallFunc(common_func_t func, void* arg); // ok
    void Start();
    void Stop();
    bool IsStop();

    static uint64_t TimerCheck();

    bool operator >(MyTimer&);
    bool operator <(MyTimer&);
    bool operator ==(MyTimer&);
    bool operator !=(MyTimer&);
private:
    uint64_t m_time;          // future time
    int m_period;             // interval
    common_func_t m_func;     // callback func
    void* m_arg;              // callback arg
    bool m_isStop;            // stop flag
    std::mutex m_mutex;       //
    static std::multimap<uint64_t,MyTimer*> l_timers;
    static std::mutex l_mutex;
    static void CallFunc(my_master::MyEvent*);
};

#endif // MYTIMER_H
