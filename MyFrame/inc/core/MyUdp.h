#ifndef MYUDP_H
#define MYUDP_H
#include "MyCommon.h"
#include "MySock.h"
#include "MyNet.h"
#include <mutex>

NS_MYFRAME_BEGIN

/*  udp package size  : suggest < 567 byte
 */

#define RECV_SIZE 65535
class MyUdp : public myframe::MySock
{
public:
    // you need write you local ip and port
    MyUdp(std::string ip,uint16_t port);
    virtual ~MyUdp(){}
    ////////////////////////////////////////////////
    /// override virtual method
    virtual uint32_t GetEpollEventType(){ return EPOLLIN; }

    MyAddrInfo RecvData(char** buf, int& len);
    int Write(MyAddrInfo &info,const char* buf, int len);

    int SetBoardCast();
protected:
private:
    char m_buf[RECV_SIZE];
    std::mutex m_mutex;
};

NS_MYFRAME_END // end namespace
#endif // MYUDP_H














