#ifndef MYSOCK_H
#define MYSOCK_H
#include "Common.h"
#include "MyEvent.h"
#include "MyNet.h"
namespace my_master {

// only IPv4
class MySock : public MyEvent
{
    friend class MyApp;
public:
public:
    MySock(std::string ip,uint16_t port,int type, int protol = 0);
    virtual ~MySock();
    //////////////////////////////////// override MyEvent method
    int GetEventFd(){ return m_sock; }
    EVENT_TYPE GetEventType(){ return EVENT_TYPE::SOCKFD; }
    uint32_t GetEpollEventType(){ return EPOLLIN; }
    virtual void* CallBackFunc(MyEvent *);
    virtual std::string GetClassType(){return "MySock";}
public:
    int Bind();
    int Close();
    int SetReuseSock();
    int SetNonblock(bool b = true);
protected:
    int m_sock;                    // socket file descritor.
    std::string m_ip;              // if server, bind ip; if client, connect ip.
    uint16_t m_port;               // if server, bind port; if client, connect port.
    struct sockaddr_in m_addr;     // if server, save self addr; if client, save client addr.
private:
    int Socket(int domin, int type, int protocol);
};

} // end namespace
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
