#include "MyTcp.h"
#include "MyApp.h"
#include "MyAllEvent.h"
using namespace my_master;
////////////////////////////////////////////////////
/// MyTcpServer
MyTcpServer::MyTcpServer(std::string ip, uint16_t port)
    :MySock(ip,port,SOCK_STREAM,true)
{}
MyTcpServer::~MyTcpServer()
{}
int MyTcpServer::Listen(int backlog)
{
    if(!IS_SERVER(m_tcp_ip_type))
        return 0;
    int res = -1;
    res = listen(m_sock,backlog);
    assert(res == 0);
    return res;
}

int MyTcpServer::Accpet(struct sockaddr_in *addr, socklen_t *addrlen)
{
    if(!IS_SERVER(m_tcp_ip_type))
        return 0;
    int res = accept(m_sock,(sockaddr*)addr,addrlen);
    return res; // file descriptor
}
////////////////////////////////////////////////////
/// MyTcpClient
MyTcpClient::MyTcpClient(std::string ip,uint16_t port)
    :MySock(ip,port,SOCK_STREAM,false)
{}
MyTcpClient::~MyTcpClient()
{}

int MyTcpClient::Read(char* buf,int len)
{
    if(IS_SERVER(m_tcp_ip_type))
        return 0;
    return read(m_sock,buf,len);
}

int MyTcpClient::Write(const char* buf, int len)
{
    if(IS_SERVER(m_tcp_ip_type))
        return 0;
    return write(m_sock,buf,len);
}
int MyTcpClient::Connect()
{
    if(IS_SERVER(m_tcp_ip_type))
        return -1;

    int ret;

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(m_port);
    inet_pton(AF_INET, m_ip.c_str(), &m_addr.sin_addr);

    ret = connect(m_sock, (struct sockaddr*)&m_addr, sizeof(m_addr));
    assert(ret == 0);
    return ret;
}

///////////////////////////////////////////////////////
/// MyTcpSocket
MyTcpSocket::MyTcpSocket(int fd, sockaddr_in addr)
{
    this->m_sock = fd;
    memcpy(&m_addr,&addr,sizeof(addr));
    Common::SetNonblock(fd,true);
}
MyTcpSocket::MyTcpSocket(const MyTcpSocket& other)
{
    memcpy(&m_addr,&other.m_addr,sizeof(m_addr));
    m_sock = other.m_sock;
}

MyTcpSocket::~MyTcpSocket()
{}

void* MyTcpSocket::CallBackFunc(MyEvent *ev)
{
    MyAllEvent::BoardEvent(ev);
    return NULL;
}

MyTcpSocket& MyTcpSocket::operator=(MyTcpSocket& other)
{
    memcpy(&m_addr,&other.m_addr,sizeof(m_addr));
    m_sock = other.m_sock;
    return *this;
}

int MyTcpSocket::Read(char* buf,int len)
{
    return read(m_sock,buf,len);
}

int MyTcpSocket::Write(const char* buf, int len)
{
    return write(m_sock,buf,len);
}
