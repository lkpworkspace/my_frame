#include "MyCapClient.h"
#include "MyUdp.h"
#include "MainWidget.h"

using namespace my_master;

MyCapClient::MyCapClient(std::string key,
                         std::string ip,
                         uint16_t port)
    :MyEasyTcpClient(ip,port),
      MyCtrlObj(key),
      m_account(""),
      m_pass(""),
      m_conn_account(""),
      m_conn_pass("")
{

}

int MyCapClient::Frame(const char *buf, int len)
{
    if(len == 0)
        return false;

    printf("client frame get buf\n");

    Handle(buf,len);
    return true;
}

void MyCapClient::Run()
{
    MyApp app(4,1024);

    app.AddEvent(this);

    app.Exec();
}

bool MyCapClient::ConnectTo(std::string dst_account, std::string dst_pass)
{
    int len = 0;
    char* buf = BuildConnectMsg(dst_account,dst_pass,&len);
    int res = EasyWrite(buf,len);
    assert(res == len);
    MySelfProtocol::FreeBuf(buf);
    return true;
}

void MyCapClient::Handle(const char* buf, int len)
{
    uint16_t head = MySelfProtocol::HandleHeader(buf);
    switch(head)
    {
    case 0x0002:
        HandleAccountAndPass(buf,len);
        break;
    case 0x0003:
        HandleConnectAcountAndPass(buf,len);
        break;
    case 0x0004:
        HandleData(buf,len);
        break;
    default:
        break;
    }
}

void MyCapClient::HandleConnectAcountAndPass(const char* buf, int len)
{
    int index = 2;
    m_conn_account = MySelfProtocol::HandleString(index,buf,len);
    index += (m_conn_account.size() + 1);
    m_conn_pass = MySelfProtocol::HandleString(index,buf,len);
    // begin cap and send TODO...
    ((MainWidget*)REQUEST("mainwidget"))->Cap();
}

void MyCapClient::HandleAccountAndPass(const char* buf, int len)
{
    int index = 2;
    m_account = MySelfProtocol::HandleString(index,buf,len);
    index += (m_account.size() + 1);
    m_pass = MySelfProtocol::HandleString(index,buf,len);
    // notify ui
    ((MainWidget*)REQUEST("mainwidget"))->GetAccount();
}

void MyCapClient::HandleData(const char* buf, int len)
{
    int index = 2;
    uint16_t data_len = 0;
    data_len = MySelfProtocol::HandleLen(index,buf,len);
    index += data_len;
    // TODO...
}

char* MyCapClient::BuildConnectMsg(std::string dst_account, std::string dst_pass, int* outlen)
{
    int index = 0;
    int len = 0;
    uint16_t head = 0x0003;
    char* buf = MySelfProtocol::GetBuf(&len);
    index += MySelfProtocol::BuildHeader(head,buf,len);
    index += MySelfProtocol::BuildString(dst_account.c_str(),index,buf,len);
    index += MySelfProtocol::BuildString(dst_pass.c_str(),index,buf,len);
    *outlen = index;
    return buf;
}
