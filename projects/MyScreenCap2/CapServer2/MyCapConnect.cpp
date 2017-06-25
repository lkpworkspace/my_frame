#include "MyCapConnect.h"
#include "MyUdp.h"

using namespace my_master;

MyCapConnect::MyCapConnect(std::string account,
                           std::string pass,
                           int fd,
                           sockaddr_in addr)
    :MyCtrlObj(account),
      MyEasyTcpSocket(fd,addr),
      m_account(account),
      m_pass(pass),
      m_conn_account(""),
      m_conn_pass("")
{
}

MyCapConnect::~MyCapConnect()
{
    UNREG(m_account);
}

int MyCapConnect::Frame(const char *buf, int len)
{
    if(len == 0)
    {
        printf("Frame: client quit\n");
        return false;
    }
    //printf("Get Frame :\n");
    Handle(buf,len);
    return true;
}

void MyCapConnect::Handle(const char* buf, int len)
{
    uint16_t head = MySelfProtocol::HandleHeader(buf);
    switch(head)
    {
    case 0x0003:
        HandleConnectMsg(buf,len);
        break;
#if 1
    case 0x0004:
        HandleData(buf,len);
        break;
    case 0xffff:
        MyDebugPrint("error in this case 0xffff\n");
        break;
    default:
        MyDebugPrint("error in this case %d\n",head);
        break;
#else
    default:
        HandleData(buf,len);
        break;
#endif
    }
}

void MyCapConnect::HandleConnectMsg(const char* buf, int len)
{
    MyCapConnect* temp;
    int index = 2;
    m_conn_account = MySelfProtocol::HandleString(index,buf,len);
    index += (m_conn_account.size() + 1);
    m_conn_pass = MySelfProtocol::HandleString(index,buf,len);
    temp = ((MyCapConnect*)REQUEST(m_conn_account));
    printf("get connect msg,dst account %s,dst pass %s\n",
           m_conn_account.c_str(),
           m_conn_pass.c_str());
    if(temp != NULL)
    {
        temp->m_conn_account = m_account;
        temp->m_conn_pass = m_pass;
        // send to client
        temp->EasyWrite(buf,len);
    }
}

void MyCapConnect::HandleData(const char* buf, int len)
{
    MyCapConnect* temp;
    if(!m_conn_account.empty())
    {
        temp = ((MyCapConnect*)REQUEST(m_conn_account));
        if(temp != NULL)
            temp->EasyWrite(buf,len);
    }
}

char* MyCapConnect::BuildMsg(uint8_t msg_num, int *outlen)
{
    uint16_t head = 0x0001;
    int len, index = 0;
    char* buf = MySelfProtocol::GetBuf(&len);
    index += MySelfProtocol::BuildHeader(head,buf,len);
    index += MySelfProtocol::BuildChar(msg_num,index,buf,len);
    *outlen = index;
    return buf;
}

char* MyCapConnect::BuildAccountAndPass(std::string account,
                                    std::string pass,
                                    int* outlen)
{
    uint16_t head = 0x0002;
    int len, index = 0;
    char* buf = MySelfProtocol::GetBuf(&len);
    index += MySelfProtocol::BuildHeader(head,buf,len);
    index += MySelfProtocol::BuildString(account.c_str(),index,buf,len);
    index += MySelfProtocol::BuildString(pass.c_str(),index,buf,len);
    *outlen = index;
    return buf;
}
