#include "MyCapConnect.h"

using namespace my_master;

MyCapConnect::MyCapConnect(std::string account,
                           std::string pass,
                           int fd,
                           sockaddr_in addr)
    :MyCtrlObj(account),
      MyTcpSocket(fd,addr),
      m_account(account),
      m_pass(pass),
      m_complete(true),
      m_head(0xffff),
      m_index(0),
      m_conn_account(""),
      m_conn_pass(""),
      data_len(0)
{
    memset(data,0,sizeof(data));
}

MyCapConnect::~MyCapConnect()
{
    UNREG(m_account);
}

void MyCapConnect::SetHeadAndIndex(uint16_t head, int index)
{
    if(head == -1)
        m_complete = true;
    else
        m_complete = false;
    m_index = index;
    m_head = head;
}
