#ifndef MYTCP_H
#define MYTCP_H
#include "MyCommon.h"
#include "MySock.h"
#include <queue>

namespace my_master {
class MyTcpSocket;
class MyTcpServer : public my_master::MySock
{
public:
    MyTcpServer(std::string ip,uint16_t port);
    virtual ~MyTcpServer();

    int Listen(int backlog = 10);
    int Accpet(struct sockaddr_in *addr);
protected:
    /////////////////////////////////
    /// virtual method
    virtual std::string GetClassType(){return "MyTcpServer";}
};

class MyTcpClient : public my_master::MySock
{
public:
    MyTcpClient(std::string ip,uint16_t port);
    virtual ~MyTcpClient();
protected:
    /////////////////////////////////
    /// virtual method
    virtual std::string GetClassType(){return "MyTcpClient";}
public:
    int Read(char* buf, int len);
    int Write(const char *buf, int len);
    int Connect();
};

class MyTcpSocket : public my_master::MyEvent
{
public:
    MyTcpSocket(int fd, sockaddr_in addr);
    MyTcpSocket(const MyTcpSocket& other);
    virtual ~MyTcpSocket();
    ////////////////////////////////////
    /// override MyEvent method
    int GetEventFd(){ return m_sock; }
    EVENT_TYPE GetEventType(){ return EVENT_TYPE::SOCKFD; }
    uint32_t GetEpollEventType(){ return EPOLLIN; }
    ////////////////////////////////////
    /// virtual method
    virtual std::string GetClassType(){return "MyTcpSocket";}
    virtual void* CallBackFunc(MyEvent *);

    MyTcpSocket& operator=(MyTcpSocket& other);

    int Read(char*, int);
    int Write(const char*,int);

    std::string GetIp();
    unsigned short GetPort();
private:
    int m_sock;
    sockaddr_in m_addr;
};

// change tcp data stream to one frame
class MyTcpFrame
{
public:
    MyTcpFrame();
    virtual ~MyTcpFrame();
    ////////////////////////////////////////////////////////
    /// \brief Frame
    /// \param buf
    /// \param len
    ///     if 0, client quit
    /// \return
    ///     if true, add event to epoll
    ///     if false, do nothing
    virtual int Frame(const char* buf, int len) = 0;
    int EasyWrite(const char* buf, uint16_t len);
protected:
    virtual int ReadBuf(char* buf, int len) = 0;
    virtual int WriteBuf(const char* buf, int len) = 0;
    virtual int GetFd() = 0;

    int GetBuf1();
    int GetBuf();
private:
    uint16_t m_len;
    bool m_iscomplete;
    char* m_buffer;
    int m_buffer_len;
    std::queue<char> m_datas;
};

class MyEasyTcpSocket : public MyTcpSocket, public MyTcpFrame
{
public:
    MyEasyTcpSocket(int fd, sockaddr_in addr);
    virtual ~MyEasyTcpSocket();

    virtual int Frame(const char* buf, int len) = 0;
private:
    virtual int ReadBuf(char* buf, int len);
    virtual int WriteBuf(const char* buf, int len);
    virtual int GetFd(){ return GetEventFd(); }
    virtual void* CallBackFunc(MyEvent *);
};

class MyEasyTcpClient : public MyTcpClient, public MyTcpFrame
{
public:
    MyEasyTcpClient(std::string ip, uint16_t port);
    virtual ~MyEasyTcpClient();

    virtual int Frame(const char* buf, int len) = 0;
private:
    virtual int ReadBuf(char* buf, int len);
    virtual int WriteBuf(const char* buf, int len);
    virtual void* CallBackFunc(MyEvent *);
};

} // end namespace
#endif // MYTCP_H













