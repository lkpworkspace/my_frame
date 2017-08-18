#include "MyTalkServer.h"
extern MyUdp* g_udp;
extern MySqlite3* g_db;

MyTalkServer::MyTalkServer()
{
    sem_init(&m_sem,0,0);
}

bool MyTalkServer::Event(MyEvent* ev)
{
    if(ev->GetClassType() == "MyUdp")
    {
#if 0
        MyDebugPrint("get udp msg\n");
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
////////////////////////////////////////////////////////////////////
/// handle msg
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
    case MYTALK_MSG:
        HandleTalkMsg(info,buf,len);
        break;
    default:
        break;
    }
}

void MyTalkServer::HandleTalkMsg(MyAddrInfo info,char* buf, int len)
{
#if 1
    printf("get msg from %s\n",info.GetIp().c_str());
#endif
    int index = MYTALK_HEAD_SIZE;
    std::string src_account = MySelfProtocol::HandleString(index,buf,len);
    index += (src_account.size() + 1);
    std::string dst_account = MySelfProtocol::HandleString(index,buf,len);
    // index += (dst_account.size() + 1);
    // std::string msg = MySelfProtocol::HandleString(index,buf,len);
    // is dst account online ?
    std::map<std::string,member_t>::iterator it = m_mem_status.find(dst_account);
    if(it != m_mem_status.end())
    {
        // send to dst account
#if 1
        MyDebugPrint("send to online friend %d\tip %s\tport %d\n",
                     it->second.online,
                     it->second.info.GetIp().c_str(),
                     it->second.info.GetPort());
#endif
        g_udp->Write(it->second.info,buf,len);
    }
}

void MyTalkServer::HandleLogin(MyAddrInfo info,char* buf, int len)
{
    // handle info
    std::string account;
    std::string password;
    int index = 0;
    account = MySelfProtocol::HandleString(MYTALK_HEAD_SIZE,buf,len);
    index += MYTALK_HEAD_SIZE + account.size() + 1;
    password = MySelfProtocol::HandleString(index,buf,len);
#if 1
    MyDebugPrint("get member %s,%s\n",account.c_str(),password.c_str());
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
        MyDebugPrint("no member...\n");
#endif
        int lenx = 0;
        char* bufx = BuildAck(0x00,&lenx);
        g_udp->Write(info,bufx,lenx);
        MySelfProtocol::FreeBuf(bufx);
    }else
    {
#if 1
        MyDebugPrint("right member\n");
#endif
        // add to online map
        if(m_mem_status.find(account) == m_mem_status.end())
        {
            member_t mem;
            mem.account = account;
            //mem.name =
            mem.online = 1;
            mem.info = info;
            m_mem_status.insert(std::make_pair(account,mem));
        }else
        {
            // modify online info
            // TODO...
        }
        // send ack
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
    MyDebugPrint("handle ack msg\n");
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
    // build online status
    std::map<std::string,member_t>::iterator it = m_mem_status.find(account);
    if(it != m_mem_status.end())
    {
        index += MySelfProtocol::BuildChar(it->second.online,index,buf,len);
        index += MySelfProtocol::BuildString(it->second.info.GetIp().c_str(),index,buf,len);
        index += MySelfProtocol::BuildLen(it->second.info.GetPort(),index,buf,len);
    }
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
#if 1
            MyDebugPrint("get friend %s,%s,%s\n",
                   myfriend.account.c_str(),
                   myfriend.name.c_str(),
                   myfriend.mark.c_str());
#endif
        // is online
        std::map<std::string,member_t>::iterator it = m_mem_status.find(myfriend.account);
        if(it != m_mem_status.end())
        {
//            myfriend.online = it->second.online;
//            myfriend.ip = it->second.info.GetIp();
//            myfriend.port = it->second.info.GetPort();
#if 1
            MyDebugPrint("online friend %d\tip %s\tport %d\n",
                         it->second.online,
                         it->second.info.GetIp().c_str(),
                         it->second.info.GetPort());
#endif
        }else
        {
            //myfriend.online = MYTALK_OFFLINE;
        }
        friends.push_back(myfriend);
    }
    return friends;
}


















