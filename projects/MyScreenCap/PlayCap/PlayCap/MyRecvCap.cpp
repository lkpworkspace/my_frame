#include "MyRecvCap.h"

MyRecvCap::MyRecvCap(MyAddrInfo info,
                     std::string key,
                     QObject *parent)
    : QObject(parent),
      MyCtrlObj(key)
{
    m_info = info;
    m_udp = new MyUdp(info.GetIp(),info.GetPort(),true);
    m_udp->Bind();
}

void MyRecvCap::PlayCap()
{
    char* buf;
    int len;
    m_udp->RecvData(&buf,len);

}
