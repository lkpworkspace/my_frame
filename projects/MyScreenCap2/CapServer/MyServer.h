#ifndef MYSERVER_H
#define MYSERVER_H
#include "MyFrame.h"
#include "MyControls.h"
#include "MyCapConnect.h"
#include <set>

using namespace my_master;

class MyServer : public my_master::MyAllEvent, public MyCtrlObj
{
public:
    MyServer();
    bool Event(MyEvent* ev);

    std::string GetAccount();
    std::string Getpass();
private:
    bool HaveAccount(std::string);
    ////////////////////////////////////
    /// \brief BuildMsg
    /// \param outlen
    /// \return build buf
    char* BuildMsg(uint8_t msg_num,int *outlen);
    char* BuildAccountAndPass(std::string,
                              std::string,
                              int* outlen);
    ///////////////////////////////////
    /// handle msg
    void Handle(MyCapConnect*,uint16_t head);
    void Handle(char* buf, int len);
    void HandleConnectMsg(MyCapConnect* sock);
    void HandleConnectMsg(char* buf, int len);
    void HandleData(MyCapConnect*sock);
    void HandleData(char* buf, int len);
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
///     head(2byte) len(2byte) account(string) password(string)
///     0x0002
/// connect msg:
///     head(2byte) len(2byte) dst account(string) dst pass(string)
///     0x0003
/// data:
///     head(2byte) data_len(2byte) data
///     0x0004
/// send data:
///     head(2byte) len(2byte) data(nbyte)
///     0x0004

#endif // MYSERVER_H
