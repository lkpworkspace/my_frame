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

int MyUdp::Write(MyAddrInfo& info,const char* buf,int len)
{
    m_mutex.lock();
    socklen_t addr_len = sizeof(sockaddr_in);
    sockaddr_in addr = info.GetAddr();
    int res = sendto(m_sock,buf,len,0,(sockaddr*)&addr,addr_len);
    assert(res == len);
    m_mutex.unlock();
    return res;
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

std::string MySelfProtocol::HandleString(int offset, char* buf, int len)
{
    char temp_buf[MYPROTO_MAX_BUF_SIZE] = {0};
    int str_len = strlen(&buf[offset]);
    memcpy(temp_buf,&buf[offset],str_len);
    return std::string(temp_buf);
}

data_t MySelfProtocol::HandleData(int offset, char* buf, int len)
{
    data_t my_data;

    my_data.len = HandleLen(offset,buf,len);
    memcpy(my_data.buf,&buf[offset + sizeof(uint16_t)],my_data.len);
    return my_data;
}

////////////////////////////////////////////////
/// build
int MySelfProtocol::BuildHeader(uint16_t head, char* buf, int len)
{
    memset(buf,0,len);
    memcpy(buf,&head,sizeof(head));
    return sizeof(head);
}

int MySelfProtocol::BuildLen(uint16_t datalen, int offset, char* buf, int len)
{
    if(offset + sizeof(datalen) > len)
        return 0;
    memcpy(&buf[offset],&datalen,sizeof(datalen));
    return sizeof(datalen);
}

int MySelfProtocol::BuildChar(uint8_t ch,int offset, char* buf, int len)
{
    if(offset + sizeof(ch) > len)
        return 0;
    memcpy(&buf[offset],&ch,sizeof(ch));
    return sizeof(ch);
}

int MySelfProtocol::BuildString(const char* str, int offset, char* buf, int len)
{
    int str_len = strlen(str);
    if(str_len + 1 + offset > len)
        return 0;
    memcpy(&buf[offset],str,str_len);
    return (str_len + 1);
}

int MySelfProtocol::BuildData(const char* data, uint16_t data_len, int offset, char* buf, int len)
{
    int len_offset = BuildLen(data_len,offset,buf,len);
    if(data_len + offset + len_offset > len)
        return 0;
    memcpy(&buf[offset + len_offset],data,data_len);
    return (data_len + len_offset);
}






















