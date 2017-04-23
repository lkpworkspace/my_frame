#include "MyTalkClient.h"
#include "MyUdp.h"

MyTalkClient::MyTalkClient()
{

}

bool MyTalkClient::Event(MyEvent* ev)
{
    if(ev->GetClassType() == MyEvent::UDPCLASS)
    {
        MyUdp* udp = (MyUdp*)ev;
        char* buf;
        int len;
        MyAddrInfo info = udp->RecvData(&buf,len);
        printf("addr %s:%d\tmsg: %s\n",
               info.GetIp().c_str(),
               info.GetPort(),
               buf);
        MyApp::theApp->AddEvent(ev);
    }
    return true;
}
