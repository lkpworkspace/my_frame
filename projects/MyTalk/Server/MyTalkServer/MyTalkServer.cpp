#include "MyTalkServer.h"
extern MyUdp* g_udp;
extern MySqlite3* g_db;

MyTalkServer::MyTalkServer()
{

}

bool MyTalkServer::Event(MyEvent* ev)
{
    if(ev->GetClassType() == MyEvent::UDPCLASS)
    {
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
    }
    // send cur account's friend msg
    // TODO...
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
























