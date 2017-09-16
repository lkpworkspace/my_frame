#include "../inc/MySock.h"
#include <fcntl.h>
#include <assert.h>
#include "../inc/MyEvent.h"
#include "../inc/MyAllEvent.h"
#include "../inc/MyLog.h"
using namespace my_master;
MySock::MySock(std::string ip, uint16_t port, int type, int protol)
    :m_sock(-1),
      m_port(port),
      m_ip(ip)
{
    memset(&m_addr,0,sizeof(struct sockaddr_in));
    m_sock = Socket(AF_INET,type,protol);
}

MySock::~MySock(){
    MyDebugPrint("sock %d close\n",m_sock);
    Close();
}

void* MySock::CallBackFunc(MyEvent * ev)
{
    MyAllEvent::BoardEvent(ev);
    return NULL;
}

/////////////////////////////////////////////////// share
int MySock::SetReuseSock()
{
    return MyNet::SetReuseSock(m_sock);
}

int MySock::SetNonblock(bool b)
{
    return Common::SetNonblock(m_sock,b);
}
int MySock::Socket(int domin, int type, int protocol)
{
    int sock = socket(domin,type,protocol);
    if(sock == -1)
    {
        MyDebugPrint("create socket fail\n");
        MyError("create socket");
    }
    return sock;
}
int MySock::Bind()
{
    int res;

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(m_port);
    if(!m_ip.empty())
        inet_pton(AF_INET, m_ip.c_str(), &m_addr.sin_addr);
    else
        m_addr.sin_addr.s_addr = INADDR_ANY;

    res = bind(m_sock, (struct sockaddr*)&m_addr, sizeof(m_addr));
    if(res == -1)
    {
        MyDebugPrint("bind socket fail\n");
        MyError("bind");
    }
    return res;
}
int MySock::Close()
{
    int res = close(m_sock);
    if(res == -1)
    {
        MyDebugPrint("close socket fail\n");
        MyError("close socket");
    }
    return res;
}
