#include "MyMsgConnect.h"
#include "MyMsgServer.h"
#include "MyMsgManager.h"

using namespace my_master;

MyMsgConnect::MyMsgConnect(int fd, sockaddr_in addr)
    :MyEasyTcpSocket(fd,addr)
{
    m_buf = MySelfProtocol::GetBuf(&m_buf_size);
    m_isLogin = false;
    memset(&m_ol,0,sizeof(m_ol));
    memset(&m_all,0,sizeof(m_all));
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
        GetMsgServer()->GetManager()->RemoveConnect(this);
        MyApp::theApp->DelLater(this,1000 * 5);
        return false;
    }
    return Handle(buf,len);
}

int MyMsgConnect::Handle(const char* buf, int len)
{
    int ret = true;
    uint16_t head = MySelfProtocol::HandleHeader(buf);
    char* temp_buf = NULL;
    int temp_len = 0;

    MyDebugPrint("Msg coming len %d\n",len);
    switch(head)
    {
    case 0x0000:
        break;
    case 0x0001: /// login(ok)
        ret = HandleLogin(buf,len);
        break;
    case 0x0003: // reg account
        temp_buf = BuildAnswer(REQ_NONE,ERR_NOFUNC,&temp_len);
        this->EasyWrite(temp_buf,temp_len);
        break;
    case 0x0005: // modify account info
        temp_buf = BuildAnswer(REQ_NONE,ERR_NOFUNC,&temp_len);
        this->EasyWrite(temp_buf,temp_len);
        break;
    case 0x0009: /// msg 1(ok)
        if(m_isLogin)
            ret = HandleSingleMsg(buf,len);
        break;
    case 0x000b: // msg 2
        temp_buf = BuildAnswer(REQ_NONE,ERR_NOFUNC,&temp_len);
        this->EasyWrite(temp_buf,temp_len);
        break;
    case 0x000d: // msg 3
        temp_buf = BuildAnswer(REQ_NONE,ERR_NOFUNC,&temp_len);
        this->EasyWrite(temp_buf,temp_len);
        break;
    case 0x000f: /// client request(ok)
        if(m_isLogin)
            ret = HandleRequest(&head, buf,len);
        break;
    case 0xffff: // msg err
        temp_buf = BuildAnswer(REQ_NONE,ERR_NOFUNC,&temp_len);
        this->EasyWrite(temp_buf,temp_len);
        break;
    default:
        temp_buf = BuildAnswer(REQ_NONE,ERR_FORMAT,&temp_len);
        this->EasyWrite(temp_buf,temp_len);
        break;
    }
    return ret;
}

int MyMsgConnect::HandleLogin(const char* buf, int len)
{
    std::string account;
    std::string password;
    int index = 0;

    account = MySelfProtocol::HandleString(MSG_HEAD_SIZE,buf,len);
    index += MSG_HEAD_SIZE + account.size() + 1; // +1 is end of str('\0')
    password = MySelfProtocol::HandleString(index,buf,len);

    MyDebugPrint("get client account %s,%s\n",account.c_str(),password.c_str());

    // check is right
    return InitAccountInfo(account,password);
}

int MyMsgConnect::HandleSingleMsg(const char* buf, int len)
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
        char* send_buf = BuildAnswer(REQ_SINMSG, ERR_NOMEMBER,&outlen);
        this->EasyWrite(send_buf,outlen);
    }else
        dst->EasyWrite(buf,len);
    return true;
}

int MyMsgConnect::HandleRequest(unsigned short* re, const char* buf, int len)
{
    return true;
    uint16_t req_num = MySelfProtocol::HandleShort(MSG_HEAD_SIZE,buf,len);
    char* temp_buf = NULL;
    int temp_len = 0;

    switch(req_num)
    {
    case REQ_ALLFRIEND:
    {
        if(m_all.index != 0)
        {
            temp_buf = BuildAnswer(REQ_ALLFRIEND,ERR_REPEATREQ,&temp_len);
            this->EasyWrite(temp_buf,temp_len);
            return true;
        }
        /// loop send all friend TODO...
        temp_buf = BuildAnswer(REQ_ALLFRIEND,ERR_OK,&temp_len);
        temp_buf = BuildAllFriends(temp_buf,temp_len,&temp_len);
        this->EasyWrite(temp_buf,temp_len);
    }
        break;
    case REQ_ONLINEFRIEND:
    {
        if(m_ol.index != 0)
        {
            temp_buf = BuildAnswer(REQ_ONLINEFRIEND,ERR_REPEATREQ,&temp_len);
            this->EasyWrite(temp_buf,temp_len);
            return true;
        }
        /// loop send ol friend TODO...
        temp_buf = BuildAnswer(REQ_ONLINEFRIEND,ERR_OK,&temp_len);
        temp_buf = BuildOLFriends(temp_buf,temp_len,&temp_len);
        this->EasyWrite(temp_buf,temp_len);
    }
        break;
    case REQ_SEARCHFRIENDID:
        break;
    default:
        *re = req_num;
        return false;
    }
    return true;
}

char* MyMsgConnect::BuildOLFriends(char* buf, int offset, int* outlen)
{
    int index = offset;

    return buf;
}

char* MyMsgConnect::BuildAllFriends(char *buf, int offset, int* outlen)
{
    std::vector<AccountInfo_t*>* group = NULL;
    std::vector<AccountInfo_t*>::iterator begin;
    std::vector<AccountInfo_t*>::iterator end;

    group = GetMsgServer()->GetManager()->GetGroupMembers(m_server,m_group);
    for(;begin != end;)
    {
    }
    return buf;
}



//char* MyMsgConnect::BuildAnswer(EnumMsgCode_t err_num, int* outlen)
//{
//    int index = 0;
//    unsigned short err = err_num;

//    index += MySelfProtocol::BuildHeader(0xffff,m_buf,m_buf_size);
//    index += MySelfProtocol::BuildShort(err,index,m_buf,m_buf_size);
//    *outlen = index;
//    return m_buf;
//}

char* MyMsgConnect::BuildAnswer(EnumMsgRequest_t re ,EnumMsgCode_t err_num, int* outlen)
{
    int index = 0;
    unsigned short err = err_num;
    unsigned short request = re;

    index += MySelfProtocol::BuildHeader(0xffff,m_buf,m_buf_size);
    index += MySelfProtocol::BuildShort(request,index,m_buf,m_buf_size);
    index += MySelfProtocol::BuildShort(err,index,m_buf,m_buf_size);
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
    if(conn == NULL) return NULL;
    m_friends.Insert(conn->m_id,conn);
    return conn;
}

void MyMsgConnect::MemberQuit(std::string name)
{
    m_mutex.lock();
    m_friends.Remove(name);
    m_mutex.unlock();
    MyDebugPrint("%s remove from %s's map\n", name.c_str(),m_id.c_str());
}

int MyMsgConnect::InitAccountInfo(std::string id, std::string password)
{
    MySqlite3* db = GetMySqlite3();
    std::vector<std::string>* row;
    std::string sql;
    int len = 0;
    char* buf = NULL;

    if(m_isLogin)
    {
        MyDebugPrint("Account %s has already login\n",m_id.c_str());
        len = 0;
        buf = BuildAnswer(REQ_LOGIN, ERR_ALREADYLOGIN,&len);
        EasyWrite(buf,len);
        return true;
    }

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
            buf = BuildAnswer(REQ_LOGIN, ERR_PASSWORD,&len);
            EasyWrite(buf,len);
            return true;
        }
        m_name = begin[2];
        m_group = begin[3];
        m_lv = begin[4];
        m_server = begin[5];
        for(int i = 0; i < row->size(); ++i)
            printf("%s\t",begin[i].c_str());
        printf("\n");

        MyMsgConnect* conn = NULL;
        if((conn = GetMsgServer()->GetManager()->GetConnect(m_server,m_group,m_id)) != NULL)
        {
            MyDebugPrint("Account %s has already login\n",m_id.c_str());
            len = 0;
            buf = BuildAnswer(REQ_LOGIN, ERR_ALREADYLOGIN,&len);
            EasyWrite(buf,len);
            MyApp::theApp->DelLater(this,1000 * 5);
            return true;
        }

        MyDebugPrint("%s login success\n",id.c_str());
        GetMsgServer()->GetManager()->InsertConnect(this);
        m_isLogin = true;
    }else
    {
        // ERR_ACCOUNT
        MyDebugPrint("%s login err, do not have this id!\n",id.c_str());
        len = 0;
        buf = BuildAnswer(REQ_LOGIN, ERR_NOACCOUNT,&len);
        EasyWrite(buf,len);
        return true;
    }
    // ERR_OK
    len = 0;
    buf = BuildAnswer(REQ_NONE, ERR_OK,&len);
    EasyWrite(buf,len);
    return true;
}


