#ifndef MYSOCK_H
#define MYSOCK_H
#include "Common.h"
#include "MyEvent.h"
#include "MyNet.h"
namespace my_master {

/*
* type: low ----> height
*    1bit: 0 server; 1 client;
*    2bit: 1 tcp
*    3bit: 1 udp
*/
#define IS_SERVER(type) ((type) & 0x01)
#define IS_TCP(type) ((type) & 0x02)
#define IS_UDP(type) ((type) & 0x04)
// only IPv4
class MySock : public MyEvent
{
    friend class MyApp;
public:
public:
    MySock(std::string ip,uint16_t port,int type, bool isServer = true);
    ~MySock(){Close();}
    //////////////////////////////////// override MyEvent method
    int GetEventFd(){ return m_sock; }
    EVENT_TYPE GetEventType(){ return EVENT_TYPE::SOCKFD; }
    uint32_t GetEpollEventType(){ return EPOLLIN; }
    virtual void* CallBackFunc(MyEvent *);
    virtual CLASS_TYPE GetClassType(){return CLASS_TYPE::BASECLASS;}
public:
    int Bind();
    int Close();
    int SetNonblock(bool b = true);
    uint8_t GetSocketType(){return m_tcp_ip_type;}
protected:
    uint8_t m_tcp_ip_type;         // tcp/udp or server/client.
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
