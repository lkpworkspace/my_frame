#include "MyUdp.h"
using namespace my_master;
MyUdp::MyUdp(std::string ip,uint16_t port, bool isServer)
    :MySock(ip,port,SOCK_DGRAM,isServer)
{
    memset(m_buf,0,RECV_SIZE);
}
MyAddrInfo MyUdp::RecvData(char** buf, int& len)
{// TODO...
    sockaddr_in addr;
    socklen_t addr_len = sizeof(sockaddr_in);

    memset(m_buf,0,RECV_SIZE);
    int size = recvfrom(m_sock,m_buf,RECV_SIZE,0,(sockaddr*)&addr,&addr_len);
    *buf = m_buf;
    len = size;
    return MyAddrInfo(addr);
}

int MyUdp::Write(MyAddrInfo& info,char*buf,int len)
{
    socklen_t addr_len = sizeof(sockaddr_in);
    sockaddr_in addr = info.GetAddr();
    return sendto(m_sock,buf,len,0,(sockaddr*)&addr,addr_len);
}

