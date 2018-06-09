#ifndef MYTCP_H
#define MYTCP_H
#include "MyCommon.h"
#include "MySock.h"
#include <queue>

NS_MYFRAME_BEGIN

class MyTcpSocket;
class MyTcpServer : public myframe::MySock
{
public:
    MyTcpServer(std::string ip,uint16_t port);
    virtual ~MyTcpServer();

    int Listen(int backlog = 10);
    int Accpet(MyAddrInfo& addrinfo);
};

class MyTcpClient : public myframe::MySock
{
public:
    MyTcpClient(std::string ip,uint16_t port);
    virtual ~MyTcpClient();
public:
    int Read(char* buf, int len);
    int Write(const char *buf, int len);
    int Connect();
};

class MyTcpSocket : public myframe::MyEvent
{
public:
    MyTcpSocket(int fd, sockaddr_in addr);
    MyTcpSocket(const MyTcpSocket& other);
    virtual ~MyTcpSocket();
    ////////////////////////////////////
    /// override MyEvent method
    virtual int GetEventFd() final { return m_sock; }
    virtual EVENT_TYPE GetEventType() final { return EVENT_TYPE::EV_SOCKFD; }
    virtual uint32_t GetEpollEventType() final { return EPOLLIN; }

    MyTcpSocket& operator=(MyTcpSocket& other);

    int Read(char*, int);
    int Write(const char*,int);

    std::string GetIp();
    unsigned short GetPort();
protected:
    ////////////////////////////////////
    /// virtual method
    virtual void* CallBackFunc(MyEvent *);

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
    int GetBufx();
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
    virtual int GetFd(){ return GetEventFd(); }
    virtual int ReadBuf(char* buf, int len);
    virtual int WriteBuf(const char* buf, int len);
    virtual void* CallBackFunc(MyEvent *);
};

NS_MYFRAME_END // end namespace
#endif // MYTCP_H













