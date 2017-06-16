#ifndef MYCAPCONNECT_H
#define MYCAPCONNECT_H
#include "MyFrame.h"
#include "MyTcp.h"
#include "MyControls.h"

using namespace my_master;
class MyServer;
class MyCapConnect : public MyCtrlObj, public my_master::MyTcpSocket
{
    friend class MyServer;
public:
    MyCapConnect(std::string account,
                 std::string pass,
                 int fd,
                 sockaddr_in addr);
    virtual ~MyCapConnect();
    std::string GetAccount() { return m_account; }
    std::string GetPass() { return m_pass; }

    bool IsComplete(){ return m_complete; }
    uint16_t GetHead() { return m_head;}
    int GetIndex(){ return m_index; }

    void SetHeadAndIndex(uint16_t head, int index);
    std::string m_conn_account;
    std::string m_conn_pass;
    char data[65535];
    int data_len;
private:
    std::string m_account;
    std::string m_pass;

    bool m_complete;
    uint16_t m_head;
    int m_index;
};

#endif // MYCAPCONNECT_H
