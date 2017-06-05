#include "MySendCap.h"

MySendCap::MySendCap(std::__cxx11::string key, MyAddrInfo info)
    :MyCtrlObj(key),
      m_info(info)
{
    m_udp = new MyUdp("",9999,false);
}

void MySendCap::Send(char* data, int len)
{
    m_udp->Write(m_info,data,len);
}
