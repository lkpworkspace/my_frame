#include "../inc/MyApp.h"
using namespace my_master;

std::multimap<uint64_t,MyTimer*> MyTimer::l_timers;
std::mutex MyTimer::l_mutex;

MyTimer::MyTimer(uint64_t period)
{
    m_isStop = true;
    m_period = period;
    m_func = nullptr;
    m_time = 0x0;
}

MyTimer::~MyTimer()
{}

void MyTimer::SetCallFunc(common_func_t func, void *arg)
{
    m_func = func;
    m_arg = arg;
}

void MyTimer::CallFunc(my_master::MyEvent* ev)
{
    MyTimer* mt = (MyTimer*)ev;
    MyDebugPrint("Timer was called\n");
    if(mt->m_func != nullptr)
        mt->m_func(mt->m_arg);
}

void MyTimer::Start()
{
    m_mutex.lock();
    m_isStop = false;
    m_time = MyHelp::GetTimerNow() + m_period;
    m_mutex.unlock();
    // insert into l_timers
    l_mutex.lock();
    l_timers.insert(std::pair<uint64_t, MyTimer*>(m_time, this));
    l_mutex.unlock();
    // to work
    MyApp::theApp->GetNormalEvent()->Work();
}

void MyTimer::Stop()
{
    m_mutex.lock();
    m_isStop = true;
    m_mutex.unlock();
}

bool MyTimer::IsStop()
{
    return m_isStop;
}

uint64_t MyTimer::TimerCheck()
{
#if 1
    std::vector<timer_iter> temp_iter;
    std::vector<timer_iter>::iterator temp_begin;
    std::vector<timer_iter>::iterator temp_end;
    l_mutex.lock();
    while (l_timers.size())
    {
        MyTimer* timer;
        uint64_t temp = 0;
        std::pair<timer_iter, timer_iter> range;

        // 响应最近的定时器
        range = l_timers.equal_range(l_timers.begin()->first);
        for (timer_iter iter = range.first;
             iter != range.second;
             ++iter)
        {
            timer = iter->second;
            temp = timer->m_time;

            if ((timer != NULL) && (timer->m_time <= MyHelp::GetTimerNow()))
            {
                if(!timer->IsStop())
                {
                    timer->CallFunc((MyEvent*)timer);
                    /// uint64_t test = timer->m_time;
                    timer->m_time += timer->m_period;
                    temp_iter.push_back(iter); // 删除旧的时间的定时器
                    l_timers.insert(std::pair<uint64_t, MyTimer*>(
                        timer->m_time, timer)); // 插入新的时间的定时器
                }else
                {
                    temp_iter.push_back(iter);
                }
            }else
            {
                // 删除过期的定时器
                if(!temp_iter.empty())
                {
                    for (temp_begin = temp_iter.begin(),temp_end = temp_iter.end();
                        temp_begin != temp_end;
                        ++temp_begin)
                    {
                        l_timers.erase(*temp_begin);
                    }
                    temp_iter.clear();
                }
                l_mutex.unlock();
                return (temp - MyHelp::GetTimerNow());
            }
        }
        // 删除过期的定时器
        if(!temp_iter.empty())
        {
            for (temp_begin = temp_iter.begin(),temp_end = temp_iter.end();
                temp_begin != temp_end;
                ++temp_begin)
            {
                l_timers.erase(*temp_begin);
            }
            temp_iter.clear();
        }
    }
    l_mutex.unlock();
#endif
    return 1000 * 60;
}

bool MyTimer::operator >(MyTimer& t)
{
    if(m_time > t.m_time)
        return true;
    return false;
}

bool MyTimer::operator <(MyTimer& t)
{
    if(m_time < t.m_time)
        return true;
    return false;
}

bool MyTimer::operator ==(MyTimer& t)
{
    if(m_time == t.m_time)
        return true;
    return false;
}

bool MyTimer::operator !=(MyTimer& t)
{
    if(m_time != t.m_time)
        return true;
    return false;
}
