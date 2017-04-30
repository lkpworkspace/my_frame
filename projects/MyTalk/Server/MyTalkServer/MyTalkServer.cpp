#include "MyTalkServer.h"
extern MyUdp* g_udp;
extern MySqlite3* g_db;

MyTalkServer::MyTalkServer()
{
    sem_init(&m_sem,0,0);
}

bool MyTalkServer::Event(MyEvent* ev)
{
    if(ev->GetClassType() == MyEvent::UDPCLASS)
    {
#if 0
        printf("get udp msg\n");
#endif
        MyUdp* udp = (MyUdp*)ev;
        char* buf;
        int len;
        MyAddrInfo info = udp->RecvData(&buf,len);
        HandleMsg(info,buf,len);
        MyApp::theApp->AddEvent(ev);
    }
    return false;
}

void MyTalkServer::HandleMsg(MyAddrInfo info,char* buf, int len)
{
    uint16_t header = MySelfProtocol::HandleHeader(buf);
    switch(header)
    {
    case MYTALK_LOGIN:
        HandleLogin(info,buf,len);
        break;
    case MYTALK_ACK:
        HandleAck(info,buf,len);
        break;
    default:
        break;
    }
}

void MyTalkServer::HandleLogin(MyAddrInfo info,char* buf, int len)
{
#if 1
    printf("get login msg\n");
#endif
    // handle info
    std::string account;
    std::string password;
    int index = 0;
    account = MySelfProtocol::HandleString(MYTALK_HEAD_SIZE,buf,len);
    index += MYTALK_HEAD_SIZE + account.size() + 1;
    password = MySelfProtocol::HandleString(index,buf,len);
#if 1
    printf("get member %s,%s\n",account.c_str(),password.c_str());
#endif
    // check is right
    std::string sql;
    sql += "select * from member where account = '";
    sql += account;
    sql += "' and password = '";
    sql += password;
    sql += "'";
    g_db->ExecSql(sql.c_str());

    std::vector<std::string>* row = g_db->GetRow();
    // send ack
    if(row == nullptr)
    {
#if 1
        printf("no member...\n");
#endif
        int lenx = 0;
        char* bufx = BuildAck(0x00,&lenx);
        g_udp->Write(info,bufx,lenx);
        MySelfProtocol::FreeBuf(bufx);
    }else
    {
#if 1
        printf("right member\n");
#endif
        int lenx = 0;
        char* bufx = BuildAck(0x01,&lenx);
        g_udp->Write(info,bufx,lenx);
        MySelfProtocol::FreeBuf(bufx);
        // send cur account's friend msg
        // TODO...
        int leny = 0;
        std::vector<mytalk_friend_t> friends = GetFriends(account);
        auto begin = friends.begin();
        auto end = friends.end();
        for(;begin != end; ++begin)
        {
            mytalk_friend_t b = (mytalk_friend_t)*begin;
#if 1
            printf("get friend %s,%s,%s\n",
                   b.account.c_str(),
                   b.name.c_str(),
                   b.mark.c_str());
#endif
            char* bufy = BuildFriend(b.account,b.name,b.mark,&leny);
            g_udp->Write(info,bufy,leny);

            // bad code
            //sem_wait(&m_sem);          // should not block, wtf...

            MySelfProtocol::FreeBuf(bufy);
        }
    }
}

void MyTalkServer::HandleAck(MyAddrInfo info, char* buf, int len)
{
#if 1
    printf("handle ack msg\n");
#endif
    uint8_t ack_num = MySelfProtocol::HandleChar(MYTALK_HEAD_SIZE,buf,len);
    switch(ack_num)
    {
    case 0x02:
    case 0x03:
        sem_post(&m_sem);
        break;
    }
}


//////////////////////////////////////////////////////////////////
/// build msg
char* MyTalkServer::BuildAck(char num, int* outlen)
{
    int len = 0;
    int index = 0;
    char* buf = MySelfProtocol::GetBuf(&len);
    index += MySelfProtocol::BuildHeader(MYTALK_ACK,buf,len);
    index += MySelfProtocol::BuildChar(num,index,buf,len);
    *outlen = index;
    return buf;
}

char* MyTalkServer::BuildFriend(std::string account,
                  std::string name,
                  std::string mark,
                  int* outlen)
{
    int len = 0;
    int index = 0;
    char* buf = MySelfProtocol::GetBuf(&len);
    index += MySelfProtocol::BuildHeader(MYTALK_FRIEND,buf,len);
    index += MySelfProtocol::BuildString(account.c_str(),index,buf,len);
    index += MySelfProtocol::BuildString(name.c_str(),index,buf,len);
    index += MySelfProtocol::BuildString(mark.c_str(),index,buf,len);
    *outlen = index;
    return buf;
}


//////////////////////////////////////////////////////////////////////////////
std::vector<mytalk_friend_t> MyTalkServer::GetFriends(std::string account)
{
    std::string sql;
    sql += "select  member.* from member, friend where member.account=friend.friend and friend.account='";
    sql += account;
    sql += "'";
    g_db->ExecSql(sql.c_str());
    std::vector<mytalk_friend_t> friends;
    mytalk_friend_t myfriend;
    std::vector<std::string>* row;
    while((row = g_db->GetRow())!= nullptr)
    {
        auto begin = row->begin();
        myfriend.account = begin[0];
        myfriend.name = begin[1];
        myfriend.mark = begin[3];
        friends.push_back(myfriend);
    }
    return friends;
}


















