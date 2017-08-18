#include "../inc/MyCapClient.h"
#include "MyUdp.h"
#include "../inc/MainWidget.h"

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
    m_buf = new char[1024* 1024 * 10];
    m_index = 0;
}

int MyCapClient::Frame(const char *buf, int len)
{
    if(len == 0)
        return false;

    //printf("client frame get buf\n");

    Handle(buf,len);
    return true;
}

void MyCapClient::EasySendPic(const char* data, int len)
{
#define USE_HEAD
    // head(2byte) data_len(2byte) data(len)
    int index = 0;
    int out_len = 0;
    int count = len / 1024;
    int last = len % 1024;
    char* buf = new char[len];

    memset(buf,0,len);
    memcpy(buf,data,len);

    for(int i = 0; i < count; ++i)
    {
#ifdef USE_HEAD
        char* temp = BuildData(&buf[index],1024,&out_len);
        EasyWrite(temp,out_len);
        MySelfProtocol::FreeBuf(temp);
#else
        EasyWrite(&buf[index],1024);
#endif
        index += 1024;
        usleep(5);
    }
    if(last >= 0)
    {
#ifdef USE_HEAD
        char* temp = BuildData(&buf[index],last,&out_len);
        EasyWrite(temp,out_len);
        MySelfProtocol::FreeBuf(temp);
#else
        EasyWrite(&buf[index],last);
#endif
    }
    delete[] buf;
}

char* MyCapClient::BuildData(const char* buf, uint16_t len, int* outlen)
{
    int index = 0;
    int out_len = 0;
    uint16_t head = 0x0004;

    char* bufx = MySelfProtocol::GetBuf(&out_len);
    index += MySelfProtocol::BuildHeader(head,bufx,out_len);
    index += MySelfProtocol::BuildLen(len,index,bufx,out_len);
    if(len > 0)
    {
        memcpy(&bufx[index],buf,len);
        index += len;
    }
    *outlen = index;
    return bufx;
}

void MyCapClient::Run()
{
    MyApp app(4);

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
    case 0x0001:
        HandleMsg(buf,len);
        break;
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
        MyDebugPrint("should not in this case\n");
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
    index += 2;
    // TODO...
    if(data_len > 0)
        memcpy(&m_buf[m_index],&buf[index],data_len);
    m_index += data_len;

    if(data_len != 1024)
    {
        //qDebug("%d\n",m_index);
        QByteArray ba(m_buf,m_index);
        ((MainWidget*)REQUEST("mainwidget"))->GetPic(ba);
        m_index = 0;
    }
}

void MyCapClient::HandleMsg(const char* buf, int len)
{
    int index = 2;
    uint8_t msg_num = MySelfProtocol::HandleChar(index,buf,len);
    switch (msg_num) {
    case 0x03: // connect client quit
        ((MainWidget*)REQUEST("mainwidget"))->EndCap();
        break;
    default:
        break;
    }
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
