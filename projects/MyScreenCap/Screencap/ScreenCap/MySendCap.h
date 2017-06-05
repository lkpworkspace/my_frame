#ifndef MYSENDCAP_H
#define MYSENDCAP_H
#include "MyControls.h"
#include "MyFrame.h"
#include "MyUdp.h"
using namespace my_master;

class MySendCap : public MyCtrlObj
{
public:
    MySendCap(std::string key, MyAddrInfo info);

    void Send(const char *data, int len);

private:
    MyUdp* m_udp;
    MyAddrInfo m_info;
};

#endif // MYSENDCAP_H
