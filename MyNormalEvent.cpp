#include "MyNormalEvent.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <MyApp.h>

using namespace my_master;

MyNormalEvent::MyNormalEvent()
{
    m_msgFd[0] = -1;
    m_msgFd[1] = -1;
    memset(m_msgBuf,0,1);
    CreateSockPair();
    MyApp::theApp->AddEvent(this);
}

MyNormalEvent::~MyNormalEvent()
{
    Close();
}

void* MyNormalEvent::CallBackFunc(MyEvent *ev)
{
    MyNormalEvent* ne = (MyNormalEvent*)ev;
    char *buf;

    ne->RecvMsg(&buf,MSG_LEN);
    MyApp::theApp->AddEvent(ev);
    return NULL;
}

int MyNormalEvent::Work()
{
    char ch  = 'y';
    return SendMsg(&ch,MSG_LEN);
}

int MyNormalEvent::RecvMsg(char** buf, int len)
{
    return read(m_msgFd[1],buf,len);
}

int MyNormalEvent::SendMsg(const char* buf, int len)
{
    return write(m_msgFd[0],buf,len);
}

int MyNormalEvent::CreateSockPair()
{
    int res = -1;
    res = socketpair(AF_UNIX,SOCK_DGRAM,0,m_msgFd);
    assert(res != -1);
    Common::SetNonblock(m_msgFd[0],true);
    Common::SetNonblock(m_msgFd[1],true);
    return res;
}
