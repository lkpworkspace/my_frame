#include "MySendCap.h"
#include "MyUdp.h"

MySendCap::MySendCap(std::__cxx11::string key, MyAddrInfo info)
    :MyCtrlObj(key),
      m_info(info)
{
    m_udp = new MyUdp("",9999,false);
}

void MySendCap::Send(const char* data, int len)
{
    //return;
    int index = 0;
    int count = len / 1024;
    int last = len % 1024;
    char* buf = new char[len];

    memset(buf,0,len);
    memcpy(buf,data,len);

    for(int i = 0; i < count; ++i)
    {
        m_udp->Write(m_info,&buf[index],1024);
        index += 1024;
        usleep(5);
    }
    if(last > 0)
    {
        m_udp->Write(m_info,&buf[index],last);
    }
    delete[] buf;
}
