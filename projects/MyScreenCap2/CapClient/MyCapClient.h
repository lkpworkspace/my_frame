#ifndef MYCAPCLIENT_H
#define MYCAPCLIENT_H

#include "MyFrame.h"
#include "MyTcp.h"
#include "MyControls.h"
#include "MyThread.h"

using namespace my_master;

class MyCapClient : public MyEasyTcpClient, public MyCtrlObj, public MyThread
{
public:
    MyCapClient(std::string key,
                std::string ip,
                uint16_t port);
    virtual ~MyCapClient(){}

    virtual int Frame(const char *buf, int len);

    void EasySendPic(const char *data, int len);
    std::string GetAccount(){return m_account;}
    std::string GetPass(){return m_pass;}
    bool ConnectTo(std::string dst_account, std::string dst_pass);
protected:
    void Run();
private:
    std::string m_account;
    std::string m_pass;
    std::string m_conn_account;
    std::string m_conn_pass;
    char* m_buf;
    int m_index;
    /////////////////////////////////////////////
    /// build
    char* BuildConnectMsg(std::string dst_account, std::string dst_pass, int* outlen);
    char* BuildData(const char* buf, uint16_t len, int *outlen);
    /////////////////////////////////////////////
    /// handle
    void Handle(const char* buf, int len);
    void HandleAccountAndPass(const char* buf, int len);
    void HandleConnectAcountAndPass(const char* buf, int len);
    void HandleData(const char* buf, int len);
};

#endif // MYCAPCLIENT_H
