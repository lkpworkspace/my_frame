#include "MyRecvCap.h"
#include <QDebug>

MyRecvCap::MyRecvCap(MyAddrInfo info,
                     std::string key,
                     QObject *parent)
    : QObject(parent),
      MyCtrlObj(key)
{
    m_info = info;
    m_udp = new MyUdp(info.GetIp(),info.GetPort(),true);
    m_udp->Bind();
    m_udp->SetNonblock(false);

    m_buf = new char[1024 * 1024 * 10];
    memset(m_buf,0,sizeof(m_buf));
    m_index = 0;

    Start();
}

void MyRecvCap::Run()
{
    char* buf;
    int len = 0;

    m_udp->RecvData(&buf,len);
    memcpy(&m_buf[m_index],buf,len);
    m_index += len;

    if(len != 1024)
    {
        qDebug("%d\n",m_index);
        QByteArray ba(m_buf,m_index);
        emit GetPic(ba);

        m_index = 0;
    }
}

void MyRecvCap::RecvStop()
{
    delete[] m_buf;
    Stop();
}
