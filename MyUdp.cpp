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

int MyUdp::SetBoardCast()
{
    const int opt = 1;
    //设置该套接字为广播类型
    int nb = 0;
    nb = setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
    assert(nb != -1);
    return nb;
}


////////////////////////////////////////////////////////////
/// MySelfProtocol
char *MySelfProtocol::GetBuf(int *len)
{
    if(len != NULL)
        *len = MYPROTO_MAX_BUF_SIZE;
    return (char*)malloc(MYPROTO_MAX_BUF_SIZE);
}

void MySelfProtocol::FreeBuf(char* buf)
{
    if(buf != NULL)
        free(buf);
}

////////////////////////////////////////////////
/// handle
uint16_t MySelfProtocol::HandleHeader(char* buf)
{
    uint16_t head;
    memcpy(&head,&buf[0],sizeof(uint16_t));
    return head;
}

uint16_t MySelfProtocol::HandleLen(int offset, char* buf, int len)
{
    if(offset + sizeof(uint16_t) > len)
        return 0;
    uint16_t data_len = 0;
    memcpy(&data_len,&buf[offset],sizeof(data_len));
    return data_len;
}

uint8_t MySelfProtocol::HandleChar(int offset, char* buf, int len)
{
    if(offset + sizeof(uint8_t) > len)
        return 0;
    uint8_t data_len = 0;
    memcpy(&data_len,&buf[offset],sizeof(uint8_t));
    return data_len;
}

////////////////////////////////////////////////
/// build
void MySelfProtocol::BuildHeader(uint16_t head, char* buf, int len)
{
    memset(buf,0,len);
    memcpy(buf,&head,sizeof(head));
}

bool MySelfProtocol::BuildLen(uint16_t datalen, int offset, char* buf, int len)
{
    if(offset + sizeof(datalen) > len)
        return false;
    memcpy(&buf[offset],&datalen,sizeof(datalen));
    return true;
}

bool MySelfProtocol::BuildChar(uint8_t ch,int offset, char* buf, int len)
{
    if(offset + sizeof(ch) > len)
        return false;
    memcpy(&buf[offset],&ch,sizeof(ch));
    return true;
}


























