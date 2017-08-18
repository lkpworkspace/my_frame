#include "MyTalkClient.h"
#include "MyDelegate.h"

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
    if(ev->GetClassType() == "MyUdp")
    {
        MyUdp* udp = (MyUdp*)ev;
        char* buf;
        int len;
        MyAddrInfo info = udp->RecvData(&buf,len);
#if MYTALK_DEBUG
        MyDebugPrint("get addr %s:%d\tmsg: %s\n",
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
    bool isok = false;
    sem_wait(&m_sem);
    isok = m_is_ok;
    m_is_ok = false;
    return isok;
}

int MyTalkClient::SendTalkMsg(std::string to, std::string msg)
{
    int len = 0;
#if MYTALK_DEBUG
    MyDebugPrint("src %s dst %s\n",m_account.c_str(),to.c_str());
#endif
    char* buf = BuildTalkMsg(m_account,to,msg,&len);
    g_udp->Write(m_server_info,buf,len);
    return len;
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
    case MYTALK_MSG:
        HandleTalkMsg(info,buf,len);
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
    // decode buf
    std::string account = MySelfProtocol::HandleString(MYTALK_HEAD_SIZE,buf,len);
    index += (MYTALK_HEAD_SIZE + account.size() + 1);
    std::string name = MySelfProtocol::HandleString(index,buf, len);
    index += (name.size() + 1);
    std::string mark = MySelfProtocol::HandleString(index,buf,len);
    index += (mark.size() + 1);
    uint8_t online = MySelfProtocol::HandleChar(index,buf,len);
    index += 1;
    std::string ip = MySelfProtocol::HandleString(index,buf,len);
    index += (ip.size() + 1);
    uint16_t port = MySelfProtocol::HandleLen(index,buf,len);
    // save friend info
    m_mutex.lock();
    if(m_friends.find(account) != m_friends.end())
    {
        // send repeat ack
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
    if(online == MYTALK_ONLINE)
    {
        myfriend->online = MYTALK_ONLINE;
        myfriend->info.SetIP(ip);
        myfriend->info.SetPort(port);
    }
    m_friends.insert(std::make_pair(account,myfriend));
    m_mutex.unlock();
#if MYTALK_DEBUG
    printf("get friend account %s, name %s, mark %s, ip %s:%d\n",
           account.c_str(),
           name.c_str(),
           mark.c_str(),
           myfriend->info.GetIp().c_str(),
           myfriend->info.GetPort());
#endif
    // update ui
    // problely ui init later than this function, so sleep to wait ui init
    // bad code, wtf...
    usleep(1000 * 500);
    MyDelegate::GetInstance()->AddFriend(myfriend->info,myfriend->name,myfriend->account,myfriend->mark);
    // send ok ack
    int lenx = 0;
    char* bufx = BuildAck(0x02,&lenx);
    g_udp->Write(m_server_info,bufx,lenx);
    MySelfProtocol::FreeBuf(bufx);
}
void MyTalkClient::HandleTalkMsg(MyAddrInfo info, char* buf, int len)
{
    // decode buf
    int index = MYTALK_HEAD_SIZE;
    std::string src = MySelfProtocol::HandleString(index,buf,len);
    index += (src.size() + 1);
    std::string dst = MySelfProtocol::HandleString(index,buf,len);
    index += (dst.size() + 1);
    std::string msg = MySelfProtocol::HandleString(index,buf,len);
    index += (msg.size() + 1);
#if MYTALK_DEBUG
    MyDebugPrint("%s : %s\n",src.c_str(),msg.c_str());
#endif
    // update ui
    MyDelegate::GetInstance()->AppendMsg(src,msg);
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

char* MyTalkClient::BuildTalkMsg(std::string src,
                                 std::string dst,
                                 std::string msg,
                                 int* outlen)
{
    int len = 0;
    int index = 0;
    char* buf = MySelfProtocol::GetBuf(&len);

    index += MySelfProtocol::BuildHeader(MYTALK_MSG,buf,len);
    index += MySelfProtocol::BuildString(src.c_str(),index,buf,len);
    index += MySelfProtocol::BuildString(dst.c_str(),index,buf,len);
    index += MySelfProtocol::BuildString(msg.c_str(),index,buf,len);
    *outlen = index;
    return buf;
}
