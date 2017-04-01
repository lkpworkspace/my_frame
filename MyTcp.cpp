#include "MyTcp.h"
#include "MyApp.h"

using namespace my_master;
void* callfunc_tcp_server(void* ev)
{
    // TODO... need debug
    MyTcpServer* server = (MyTcpServer*)ev;
    sockaddr_in addr;
    socklen_t len;
    MyTcpSocket* tcpSock;

    while(1)
    {
        memset(&addr,0,sizeof(addr));
        int fd = accept(server->GetEventFd(),(sockaddr*)&addr,&len);
        if(fd < 0)
        {
            MyApp::theApp->AddEvent((MyEvent*)ev);
            break;
        }
        tcpSock = new MyTcpSocket(fd,addr);
        MyApp::theApp->AddEvent(tcpSock);
    }
}
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
//MyTcpSocket MyTcpServer::GetConnect()
//{
//    sockaddr_in addr;
//    socklen_t len;
//    int fd = Accpet(&addr,&len);
//    MyTcpSocket client(fd,addr);
//    return client;
//}


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
}
MyTcpSocket::MyTcpSocket(const MyTcpSocket& other)
{
    memcpy(&m_addr,&other.m_addr,sizeof(m_addr));
    m_sock = other.m_sock;
}

MyTcpSocket::~MyTcpSocket()
{}

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
