#include "MyTalkClient.h"
std::map<std::string, mytalk_friend_t*> MyTalkClient::m_friends;
std::string MyTalkClient::m_account;
std::string MyTalkClient::m_password;
std::mutex MyTalkClient::m_mutex;
sem_t MyTalkClient::m_sem;
MyAddrInfo MyTalkClient::m_server_info;
bool MyTalkClient::m_is_ok;

MyTalkClient::MyTalkClient()
{}

bool MyTalkClient::Event(MyEvent* ev)
{
    printf("class type %d\n", ev->GetClassType());
    if(ev->GetClassType() == MyEvent::UDPCLASS)
    {
        MyUdp* udp = (MyUdp*)ev;
        char* buf;
        int len;
        MyAddrInfo info = udp->RecvData(&buf,len);
#if 1
        printf("addr %s:%d\tmsg: %s\n",
               info.GetIp().c_str(),
               info.GetPort(),
               buf);
#endif
        HandleMsg(info,buf,len);
        MyApp::theApp->AddEvent(ev);
    }
    return true;
}

void MyTalkClient::Init()
{
    sem_init(&m_sem,0,0);
    m_server_info.SetIpAndPort("127.0.0.1",4399);
    m_is_ok = false;
}

void MyTalkClient::SendLogin(std::string account, std::string password)
{
    m_account = account;
    m_password = password;
    int len;
    char *buf = MySelfProtocol::GetBuf(&len);
    int index = 0;

    index = MySelfProtocol::BuildHeader(MYTALK_LOGIN,buf,len);
    index += MySelfProtocol::BuildString(account.c_str(),index,buf,len);
    index += MySelfProtocol::BuildString(password.c_str(),index,buf,len);
    g_udp->Write(m_server_info,buf,index);
    MySelfProtocol::FreeBuf(buf);
}
bool MyTalkClient::CheckLogin()
{
#if 0
    printf("check ok befer\n");
#endif
    bool isok = false;
    sem_wait(&m_sem);
    isok = m_is_ok;
    m_is_ok = false;
#if 0
    printf("check ok end\n");
#endif
    return isok;
}

///////////////////////////////////////////////////////
/// handle msg
void MyTalkClient::HandleMsg(MyAddrInfo info, char* buf, int len)
{
    uint16_t head = MySelfProtocol::HandleHeader(buf);
    switch(head)
    {
    case MYTALK_ACK:
        HandleLogin(buf,len);
        break;
    case MYTALK_FRIEND:
        HandleFriend(info,buf,len);
        break;
    default:
        break;
    }
}

bool MyTalkClient::HandleLogin(char *buf, int len)
{
    uint8_t res = MySelfProtocol::HandleChar(MYTALK_HEAD_SIZE,buf,len);
    if(res == 0)
        m_is_ok = false;
    else
        m_is_ok = true;
    sem_post(&m_sem);
    return m_is_ok;
}

void MyTalkClient::HandleFriend(MyAddrInfo info,char* buf, int len)
{
    int index = 0;
    std::string account = MySelfProtocol::HandleString(MYTALK_HEAD_SIZE,buf,len);
    index += (MYTALK_HEAD_SIZE + account.size() + 1);
    std::string name = MySelfProtocol::HandleString(index,buf, len);
    index += (name.size() + 1);
    std::string mark = MySelfProtocol::HandleString(index,buf,len);

    if(m_friends.find(account) != m_friends.end())
    {
        // send ack
        int len = 0;
        char* buf = BuildAck(0x03,&len);
        g_udp->Write(info,buf,len);
        MySelfProtocol::FreeBuf(buf);
        return;
    }
    mytalk_friend_t* myfriend = new mytalk_friend_t;
    myfriend->account = account;
    myfriend->name = name;
    myfriend->mark = mark;
    myfriend->info = info;
    m_friends.insert(std::make_pair(account,myfriend));
#if 1
    printf("get friend %s,%s,%s\n",
           account.c_str(),
           name.c_str(),
           mark.c_str());
#endif
    // notify ui update myfriend
    // TODO...

    // send ack
    int lenx = 0;
    char* bufx = BuildAck(0x02,&lenx);
    g_udp->Write(m_server_info,bufx,lenx);
    MySelfProtocol::FreeBuf(bufx);
}

///////////////////////////////////////////////////////
/// build msg
///
char* MyTalkClient::BuildAck(char num, int* outlen)
{
    int len = 0;
    int index = 0;
    char* buf = MySelfProtocol::GetBuf(&len);
    index += MySelfProtocol::BuildHeader(MYTALK_ACK,buf,len);
    index += MySelfProtocol::BuildChar(num,index,buf,len);
    *outlen = index;
    return buf;
}
