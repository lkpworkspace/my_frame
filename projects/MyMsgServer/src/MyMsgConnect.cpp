#include "MyMsgConnect.h"
#include "MyApp.h"

using namespace my_master;

MyMsgConnect::MyMsgConnect(int fd, sockaddr_in addr)
    :MyEasyTcpSocket(fd,addr)
{

}

MyMsgConnect::~MyMsgConnect()
{}

int MyMsgConnect::Frame(const char* buf, int len)
{
    if(len == 0)
    {
        printf("Frame: %s client quit\n",m_id.c_str());
        MyApp::theApp->DelLater(this,1000 * 5);
        return false;
    }
    Handle(buf,len);
    return true;
}

void MyMsgConnect::Handle(const char* buf, int len)
{
    uint16_t head = MySelfProtocol::HandleHeader(buf);
    MyDebugPrint("Msg coming len %d\n",len);
    switch(head)
    {
    case 0x0000:
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////
/// some useful func
void MyMsgConnect::WelcomeMsg()
{
    MyDebugPrint("welcome come to my msg server\n");
}

std::string MyMsgConnect::GetAccount()
{
    std::string temp;
    char buf[9] = {0};
again:
    temp.clear();
    memset(buf,0,sizeof(buf));
    for(int i = 0; i < 9; ++i)
    {
        buf[i] = MyHelp::RandomNum(0,10);
        temp += MyHelp::ToChar(buf[i]);
    }
//    if(REQUEST(temp) != NULL)
//        goto again;
    return temp;
}

std::string MyMsgConnect::Getpass()
{
    std::string pass;
    char buf[4] = {0};

    for(int i = 0; i < 4; ++i)
    {
        buf[i] = MyHelp::RandomNum(0,10);
        pass += MyHelp::ToChar(buf[i]);
    }
    return pass;
}
