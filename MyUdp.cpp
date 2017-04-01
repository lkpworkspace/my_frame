#include "MyUdp.h"
using namespace my_master;
MyUdp::MyUdp(std::string ip,uint16_t port, bool isServer)
    :MySock(ip,port,SOCK_DGRAM,isServer)
{

}


MyUdpSocket MyUdp::RecvData()
{// TODO...
    MyUdpSocket res;
    socklen_t addr_len = sizeof(sockaddr_in);
    char buf[RECV_SIZE] = {0};

    int size = recvfrom(m_sock,buf,RECV_SIZE,0,(sockaddr*)&res.m_remote_addr,&addr_len);
    res.m_sock = m_sock;
    res.m_buf = new char[size];
    res.m_len = size;

    return res;
}

int MyUdp::Write(char*buf,int len)
{
    socklen_t addr_len = sizeof(sockaddr_in);
    return sendto(m_sock,buf,len,0,(sockaddr*)&m_addr,addr_len);
}

////////////////////////////////////////////////////////
MyUdpSocket::MyUdpSocket()
{
    memset(&m_remote_addr,0,sizeof(struct sockaddr_in));
    m_sock = -1;
    m_buf = nullptr;
    m_len = 0;
}
MyUdpSocket::MyUdpSocket(struct sockaddr_in addr,int fd)
{
    memcpy(&m_remote_addr,&addr,sizeof(struct sockaddr_in));
    m_sock = fd;
    m_buf = nullptr;
    m_len = 0;
}

MyUdpSocket::~MyUdpSocket()
{
    if(m_buf != nullptr)
        delete []m_buf;
}

MyUdpSocket& MyUdpSocket::operator=(MyUdpSocket other)
{
    if(m_buf != nullptr)
        delete[] m_buf;
    m_buf = other.m_buf;
    memcpy(&m_remote_addr,&other.m_remote_addr,sizeof(struct sockaddr_in));
    m_len = other.m_len;
    m_sock = other.m_sock;
    return *this;
}

int MyUdpSocket::GetData(char** buf)
{
    *buf = m_buf;
    return m_len;
}
