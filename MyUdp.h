#ifndef MYUDP_H
#define MYUDP_H
#include "Common.h"
#include "MySock.h"
#include "MyNet.h"
namespace my_master {

#define RECV_SIZE 4096
class MyUdp : public my_master::MySock
{
public:
    // you need write you local ip and port
    MyUdp(std::string ip,uint16_t port, bool isServer = true);
    ~MyUdp();
    ////////////////////////////////////////////////
    /// override virtual method
    virtual CLASS_TYPE GetClassType(){return CLASS_TYPE::UDPCLASS;}

    MyAddrInfo RecvData(char** buf, int& len);
    int Write(MyAddrInfo &info, char*buf, int len);
protected:
private:
    char m_buf[RECV_SIZE];
};

} // end namespace
#endif // MYUDP_H
