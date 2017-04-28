#ifndef MYUDP_H
#define MYUDP_H
#include "Common.h"
#include "MySock.h"
#include "MyNet.h"
namespace my_master {

/*  udp package size  : suggest < 567 byte
 */

#define RECV_SIZE 4096
class MyUdp : public my_master::MySock
{
public:
    // you need write you local ip and port
    MyUdp(std::string ip,uint16_t port, bool isServer = true);
    virtual ~MyUdp(){}
    ////////////////////////////////////////////////
    /// override virtual method
    virtual CLASS_TYPE GetClassType(){return CLASS_TYPE::UDPCLASS;}

    MyAddrInfo RecvData(char** buf, int& len);
    int Write(MyAddrInfo &info, char*buf, int len);

    int SetBoardCast();
protected:
private:
    char m_buf[RECV_SIZE];
    std::mutex m_mutex;
};


/////////////////////////////////////////////////////
/*  | header(2byte) | datalen(2byte) | data(0-512byte) |
 *
 */

#define MYPROTO_MAX_BUF_SIZE  1024
#define MYPROTO_HEAD_SIZE 2
// exclude 1000
#define MYPROTO_BEGIN_NUM 1000
#define MYPROTO_DATA_SIZE 512
typedef struct data_t
{
    uint16_t len;
    char buf[MYPROTO_MAX_BUF_SIZE];
}data_t;

class MySelfProtocol //: public my_master::MyUdp
{
public:
    MySelfProtocol(){}
    ~MySelfProtocol(){}

    static char* GetBuf(int* len = NULL);
    static void FreeBuf(char* buf);
    ////////////////////////////////////////////////
    /// handle
    static uint16_t HandleHeader(char* buf);
    static uint16_t HandleLen(int offset, char* buf, int len);
    static uint8_t HandleChar(int offset, char* buf, int len);
    static std::string HandleString(int offset, char* buf, int len);
    static data_t HandleData(int offset, char* buf, int len);
    ////////////////////////////////////////////////
    /// build
    static int BuildHeader(uint16_t head, char* buf, int len);
    static int BuildLen(uint16_t datalen,int offset, char* buf, int len);
    static int BuildChar(uint8_t ch,int offset, char* buf, int len);
    static int BuildString(const char* str, int offset, char* buf, int len);
    static int BuildData(const char* data, int data_len, int offset, char* buf, int len);
protected:
private:
};

} // end namespace
#endif // MYUDP_H














