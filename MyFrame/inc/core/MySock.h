#ifndef MYSOCK_H
#define MYSOCK_H
#include "MyCommon.h"
#include "MyEvent.h"
#include "MyNet.h"

NS_MYFRAME_BEGIN

// only IPv4
class MySock : public MyEvent
{
    friend class MyApp;
    friend class MyTask;
public:
    MySock(std::string ip,uint16_t port,int type, int protol = 0);
    virtual ~MySock();
    //////////////////////////////////// override MyEvent method
    virtual int GetEventFd() final { return m_sock; }
    virtual EVENT_TYPE GetEventType() override { return EVENT_TYPE::EV_SOCKFD; }
    virtual uint32_t GetEpollEventType() override { return EPOLLIN; }

    int Bind();
    int Close();
    int SetReuseSock();
    int SetNonblock(bool b = true);

    std::string& GetIp() { return m_ip; }
    uint16_t GetPort() { return m_port; }
protected:
    //////////////////////////////////// override MyEvent method
    virtual void* CallBackFunc(MyEvent *) override;

    int m_sock;                    // socket file descritor.
    std::string m_ip;              // if server, bind ip; if client, connect ip.
    uint16_t m_port;               // if server, bind port; if client, connect port.
    struct sockaddr_in m_addr;     // if server, save self addr; if client, save client addr.

private:
    int Socket(int domin, int type, int protocol);
};

NS_MYFRAME_END // end namespace
// client:
//      connect();
//      read or write
//      close();
// server:
//      bind();
//      listen();
//      accept();
//      read or write
//      close();

// socket api
//int Socket(int domin = AF_INET, int type = SOCK_STREAM, int protocol = 0);
//int Bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
//int Listen(int sockfd, int backlog);
//int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
//int Connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

#endif // MYSOCK_H
