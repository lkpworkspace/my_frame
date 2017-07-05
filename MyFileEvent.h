#ifndef MYFILEMODIFYEVENT_H
#define MYFILEMODIFYEVENT_H
#include "Common.h"
#include "MyEvent.h"

namespace my_master {
// only one instance
class MyFileEvent : public my_master::MyEvent
{
public:
    MyFileEvent();
    virtual ~MyFileEvent(){}

    static int StartMonitor(std::string path);
    static void StopMonitor(int fd);
    static void GetInfo();// read l_fd, get file info
public:
    virtual int GetEventFd(){ return l_fd; }
    virtual EVENT_TYPE GetEventType(){ return MyEvent::FILEFD; }     // maybe not most need
    virtual std::string GetClassType(){ return "MyFileEvent"; }
    virtual uint32_t GetEpollEventType(){ return EPOLLIN; }
protected:
    virtual void* CallBackFunc(MyEvent*);
private:
    static bool l_is_init;
    static int l_fd;
};

//struct inotify_event {
//    int      wd;       /* Watch descriptor */
//    uint32_t mask;     /* Mask of events */
//    uint32_t cookie;   /* Unique cookie associating related
//                          events (for rename(2)) */
//    uint32_t len;      /* Size of name field */
//    char     name[];   /* Optional null-terminated name */
//};

} // end namespace
#endif // MYFILEMODIFYEVENT_H
