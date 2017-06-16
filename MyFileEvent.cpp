#include "MyFileEvent.h"
#include "MyApp.h"
#include "MyAllEvent.h"
#include <sys/inotify.h>
#include <string.h>

using namespace my_master;
bool MyFileEvent::l_is_init = false;
int MyFileEvent::l_fd = -1;
MyFileEvent::MyFileEvent()
{
    if(!l_is_init)
    {
        l_fd = inotify_init();
        assert(l_fd != -1);
        Common::SetNonblock(l_fd,true);
        l_is_init = true;
        MyApp::theApp->AddEvent(this);
    }
}

void MyFileEvent::GetInfo()
{

    struct inotify_event *event;
    event = (struct inotify_event *)new char[1024];
    memset(event,0,1024);

    int res = read(l_fd,event,1024);
    printf("file len %d,file des %d,named %s\n",res,event->wd,event->name);

    StartMonitor("filetest.txt");
    return;
}

int MyFileEvent::StartMonitor(std::string path)
{
    int watch_fd = inotify_add_watch(l_fd,path.c_str(),IN_MODIFY);
    assert(watch_fd != -1);
    return watch_fd;
}

void MyFileEvent::StopMonitor(int fd)
{
    int res = inotify_rm_watch(l_fd,fd);
    assert(res != -1);
}

void* MyFileEvent::CallBackFunc(MyEvent* ev)
{
    MyAllEvent::BoardEvent(ev);
    MyApp::theApp->AddEvent(ev);
}
