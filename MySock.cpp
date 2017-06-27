#include "MySock.h"
#include <fcntl.h>
#include <assert.h>
#include "MyEvent.h"
#include "MyAllEvent.h"
#include "MyLog.h"
using namespace my_master;
MySock::MySock(std::string ip, uint16_t port, int type, bool isServer)
    :m_sock(-1),
      m_port(port),
      m_ip(ip),
      m_tcp_ip_type(0x00)
{
    memset(&m_addr,0,sizeof(struct sockaddr_in));
    if(isServer)
    {
        m_tcp_ip_type |= 0x01;
    }else
    {
        m_tcp_ip_type |= 0x00;
    }
    switch(type)
    {
    case SOCK_STREAM:
        m_tcp_ip_type |= 0x02;
        break;
    case SOCK_DGRAM:
        m_tcp_ip_type |= 0x04;
        break;
    default:
        break;
    }
    m_sock = Socket(AF_INET,type,0);
}

MySock::~MySock(){
    MyDebugPrint("sock close\n");
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
    assert(sock > 0);
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
    assert(res == 0);
    return res;
}
int MySock::Close()
{
    int res = close(m_sock);
    assert(res == 0);
    return res;
}
