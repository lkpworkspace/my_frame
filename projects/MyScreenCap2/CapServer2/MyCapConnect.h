#ifndef MYCAPCONNECT_H
#define MYCAPCONNECT_H
#include "MyFrame.h"
#include "MyTcp.h"
#include "MyControls.h"

using namespace my_master;
class MyServer;
class MyCapConnect : public MyCtrlObj, public my_master::MyEasyTcpSocket
{
    friend class MyServer;
public:
    MyCapConnect(std::string account,
                 std::string pass,
                 int fd,
                 sockaddr_in addr);
    virtual ~MyCapConnect();
    //////////////////////////////////////////////////
    /// override MyEasyTcpSocket method
    virtual int Frame(const char* buf, int len);

    std::string GetAccount() { return m_account; }
    std::string GetPass() { return m_pass; }
private:
    std::string m_account;
    std::string m_pass;

    std::string m_conn_account;
    std::string m_conn_pass;
private:
    ////////////////////////////////////
    /// BuildMsg
    char* BuildMsg(uint8_t msg_num,int *outlen);
    char* BuildAccountAndPass(std::string,
                              std::string,
                              int* outlen);
    ///////////////////////////////////
    /// handle msg
    void Handle(const char* buf, int len);
    void HandleConnectMsg(const char* buf, int len);
    void HandleData(const char* buf, int len);
};

///////////////////////////////////////
/// account passward
/// 9byte   4byte
/*
  maybe i need a MyTcpFrame class
  help me decode the tcp stream
*/
///////////////////////////////////////
/// build protocol
/// msg:
///     head(2byte) msg num(1byte)
///     0x0001      0x01 connect ok
///                 0x02 connect fail
/// account and password:
///     head(2byte) account(string) password(string)
///     0x0002
/// connect msg:
///     head(2byte) dst account(string) dst pass(string)
///     0x0003
/// data:
///     head(2byte) data_len(2byte) data(nbyte)
///     0x0004
#endif // MYCAPCONNECT_H
