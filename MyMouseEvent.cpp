#include "MyMouseEvent.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace my_master;

MyMouseEvent::MyMouseEvent()
{
    memset(m_buf,0,sizeof(m_buf));
    m_fd = open("/dev/input/mice", O_RDWR);
    assert(m_fd != -1);
}

MyMouseEvent::~MyMouseEvent()
{
    if(m_fd >= 0)
        close(m_fd);
}

void* MyMouseEvent::CallBackFunc(MyEvent*)
{
    int res = read(m_fd,m_buf,sizeof(m_buf));
    assert(res != -1);
}

int MyMouseEvent::GetMouseType()
{
    return (m_buf[0] & 0x07);
}

int MyMouseEvent::GetRelX()
{
    return m_buf[1];
}

int MyMouseEvent::GetRelY()
{
    return m_buf[2];
}
