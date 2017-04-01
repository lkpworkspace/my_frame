#ifndef MYUDP_H
#define MYUDP_H
#include "Common.h"
#include "MySock.h"

namespace my_master {

#define RECV_SIZE 4096

class MyUdpSocket;
class MyUdp : public my_master::MySock
{
public:
    MyUdp(std::string ip,uint16_t port, bool isServer = true);
    ~MyUdp();

    MyUdpSocket RecvData();
    int Write(char*buf,int len);
protected:
private:
};

class MyUdpSocket
{
    friend class MyUdp;
public:
    MyUdpSocket();
    MyUdpSocket(struct sockaddr_in addr,int fd);
    ~MyUdpSocket();

    MyUdpSocket& operator=(MyUdpSocket other);
    int GetData(char** buf);
private:
    char* m_buf;
    int m_len;
    int m_sock;
    struct sockaddr_in m_remote_addr;      // remote udp sockaddr
};

} // end namespace
#endif // MYUDP_H
