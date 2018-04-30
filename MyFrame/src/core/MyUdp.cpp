#include "MyUdp.h"
#include "MyLog.h"
#include "MyApp.h"

USING_MYFRAME;

MyUdp::MyUdp(std::string ip, uint16_t port)
    :MySock(ip,port,SOCK_DGRAM)
{
    memset(m_buf,0,RECV_SIZE);
}

MyAddrInfo MyUdp::RecvData(char** buf, int& len)
{
    sockaddr_in addr;
    socklen_t addr_len = sizeof(sockaddr_in);

    memset(m_buf,0,RECV_SIZE);
    int size = recvfrom(m_sock,m_buf,RECV_SIZE,0,(sockaddr*)&addr,&addr_len);
    if(size == -1)
    {
        MyDebugPrint("recvfrom fail\n");
        MyDebug("recvfrom");
    }
    *buf = m_buf;
    len = size;
    return MyAddrInfo(addr);
}

int MyUdp::Write(MyAddrInfo& info,const char* buf,int len)
{
    m_mutex.lock();
    socklen_t addr_len = sizeof(sockaddr_in);
    sockaddr_in addr = info.GetAddr();
    int res = sendto(m_sock,buf,len,0,(sockaddr*)&addr,addr_len);
    if(res == -1)
    {
        MyDebugPrint("sendto fail\n");
        MyDebug("sendto");
    }
    m_mutex.unlock();
    return res;
}

int MyUdp::SetBoardCast()
{
    const int opt = 1;
    //设置该套接字为广播类型
    int nb = 0;
    nb = setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
    if(nb == -1)
    {
        MyDebugPrint("setsockopt fail\n");
        MyError("setsockopt");
    }
    return nb;
}
























