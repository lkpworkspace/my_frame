#include "MyMsgConnect.h"
#include "MyMsgServer.h"
#include "MyMsgManager.h"

using namespace my_master;

MyMsgConnect::MyMsgConnect(int fd, sockaddr_in addr)
    :MyEasyTcpSocket(fd,addr)
{
    m_buf = MySelfProtocol::GetBuf(&m_buf_size);
    m_isLogin = false;
}

MyMsgConnect::~MyMsgConnect()
{
    MySelfProtocol::FreeBuf(m_buf);
    m_buf_size = 0;
}

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
    case 0x0001: // login
        HandleLogin(buf,len);
        break;
    case 0x0003: // reg account
        break;
    case 0x0005: // modify account info
        break;
    case 0x0009: // msg 1
        if(m_isLogin)
            HandleSingleMsg(buf,len);
        break;
    case 0x000b: // msg 2
        break;
    case 0x000d: // msg 3
        break;
    case 0x000f: // client request
        HandleRequest(buf,len);
        break;
    case 0xffff: // msg err
        HandleErr(buf,len);
        break;
    default:
        break;
    }
}

void MyMsgConnect::HandleLogin(const char* buf, int len)
{
    std::string account;
    std::string password;
    int index = 0;

    account = MySelfProtocol::HandleString(MSG_HEAD_SIZE,buf,len);
    index += MSG_HEAD_SIZE + account.size() + 1; // +1 is end of str('\0')
    password = MySelfProtocol::HandleString(index,buf,len);

    MyDebugPrint("get client account %s,%s\n",account.c_str(),password.c_str());

    // check is right
    InitAccountInfo(account,password);
}

void MyMsgConnect::HandleSingleMsg(const char* buf, int len)
{
    int index = MSG_HEAD_SIZE;
    int id_len = strlen(&buf[index]);
    index += (id_len + 1);
    std::string dst_id = MySelfProtocol::HandleString(index,buf,len);
    // search dst_id
    MyMsgConnect* dst = SearchMemberById(dst_id);
    if(dst == NULL)
    {
        int outlen = 0;
        char* send_buf = BuildErr(ERR_NOMEMBER,&outlen);
        this->EasyWrite(send_buf,outlen);
    }else
        dst->EasyWrite(buf,len);
}

void MyMsgConnect::HandleErr(const char* buf, int len)
{
    uint16_t err_num = MySelfProtocol::HandleShort(MSG_HEAD_SIZE,buf,len);
    switch(err_num)
    {
    case ERR_OK:
        MyDebugPrint("get client msg %d\n",err_num);
        break;
    default:
        break;
    }
}

void MyMsgConnect::HandleRequest(const char* buf, int len)
{
    uint16_t req_num = MySelfProtocol::HandleShort(MSG_HEAD_SIZE,buf,len);
    char* temp_buf = NULL;
    int temp_len = 0;

    switch(req_num)
    {
    case REQ_ALLFRIEND:

        break;
    case REQ_ONLINEFRIEND:

        break;
    case REQ_SEARCHFRIENDID:

        break;
    }
}

char* MyMsgConnect::BuildAnswer(EnumMsgRequest_t e, char state, int *outlen)
{
    int index = 0;
    memset(m_buf,0,m_buf_size);
    index += MySelfProtocol::BuildHeader(0x0011,m_buf,m_buf_size);
    index += MySelfProtocol::BuildShort((uint16_t)e,index,m_buf,m_buf_size);
    index += MySelfProtocol::BuildChar(state,index,m_buf,m_buf_size);
    *outlen = index;
    return m_buf;
}

char* MyMsgConnect::BuildQuit(int* outlen)
{
    int index = 0;
    char* quit_msg = NULL;
    memset(m_buf,0,m_buf_size);

    index = MySelfProtocol::BuildHeader(0x0007,m_buf,m_buf_size);
    index += MySelfProtocol::BuildString(m_id.c_str(),index,m_buf,m_buf_size);
    *outlen = index;
    return m_buf;
}

char* MyMsgConnect::BuildErr(unsigned short err_num, int* outlen)
{
    int index = 0;

    index += MySelfProtocol::BuildHeader(0xffff,m_buf,m_buf_size);
    index += MySelfProtocol::BuildShort(err_num,index,m_buf,m_buf_size);
    *outlen = index;
    return m_buf;
}

///////////////////////////////////////////////////////
/// some useful func
std::vector<std::string>& MyMsgConnect::GetAllFriendsId()
{
    MySqlite3* db = GetMySqlite3();
    std::vector<std::string>* row;
    std::string sql = "select * from \
                       MyMsgFriends \
                       where account = '";
    sql += m_id;
    sql += "'";

    db->ExecSql(sql);
    m_all_friends_id.clear();
    while((row = db->GetRow()) != NULL)
    {
        auto begin = row->begin();
        m_all_friends_id.push_back(begin[1]);
    }
    return m_all_friends_id;
}

MyMsgConnect* MyMsgConnect::SearchMemberById(std::string id)
{
    MyMsgConnect* conn = NULL;

    if(id == m_id) return this;
    m_mutex.lock();
    // search local member list
    if(m_friends.Find(id))
        conn = m_friends.Get(id);
    m_mutex.unlock();
    if(conn != NULL)
        return conn;
    // search global member list
    conn = GetMsgServer()->GetManager()->GetConnect(m_server,m_group,id);
    m_friends.Insert(conn->m_id,conn);
    return conn;
}

void MyMsgConnect::MemberQuit(std::string name)
{
    m_mutex.lock();
    m_friends.Remove(name);
    m_mutex.unlock();
    MyDebugPrint("%s remove from %s map\n", name,m_id.c_str());
}

void MyMsgConnect::InitAccountInfo(std::string id, std::string password)
{
    MySqlite3* db = GetMySqlite3();
    std::vector<std::string>* row;
    std::string sql;
    int len = 0;
    char* buf = NULL;

    sql = "select * from MyMsgAccount where account=";
    sql += id;
    db->ExecSql(sql);
    if((row = db->GetRow()) != NULL)
    {

        auto begin = row->begin();
        m_id = begin[0];
        m_pass = begin[1];
        if(m_pass != password)
        {
            // ERR_PASSWORD
            MyDebugPrint("%s login err, password err\n",id.c_str());
            len = 0;
            buf = BuildErr(ERR_PASSWORD,&len);
            EasyWrite(buf,len);
            return;
        }
        m_name = begin[2];
        m_group = begin[3];
        m_lv = begin[4];
        m_server = begin[5];
        for(int i = 0; i < row->size(); ++i)
            MyDebugPrint("%s\n",begin[i].c_str());
        MyDebugPrint("%s login success\n",id.c_str());
        GetMsgServer()->GetManager()->InsertConnect(this);
        m_isLogin = true;
    }else
    {
        // ERR_ACCOUNT
        MyDebugPrint("%s login err, do not have this id!\n",id.c_str());
        len = 0;
        buf = BuildErr(ERR_NOACCOUNT,&len);
        EasyWrite(buf,len);
        return;
    }
    // ERR_OK
    len = 0;
    buf = BuildErr(ERR_OK,&len);
    EasyWrite(buf,len);
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
