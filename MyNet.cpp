#include "MyNet.h"
#include <assert.h>
using namespace my_master;
/*
 *  net error: h_error
 *      HOST_NOT_FOUND
 *      TRY_AGAIN
 *      NO_RECOVERY
 *      NO_DATA
 *
 */

MyNet::MyNet()
{}

MyNet::~MyNet()
{}

std::string MyNet::GetAddrIp(sockaddr_in* addr)
{
    char ip_buf[64] = {0};
    inet_ntop(AF_INET,&(addr->sin_addr),ip_buf,sizeof(ip_buf));
    return std::string(ip_buf);
}

unsigned short MyNet::GetAddrPort(sockaddr_in* addr)
{
    return ntohs(addr->sin_port);
}

MyAddrInfo MyNet::GetSockInfo(const int fd)
{
    struct sockaddr_in addr;
    socklen_t len = 0;
    int res = 0;

    memset(&addr,0,sizeof(addr));
    res = getsockname(fd,(sockaddr*)&addr,&len);
    assert(res != -1);
    return MyAddrInfo(addr);
}

MyAddrInfo MyNet::GetRemoteSockInfo(const int fd)
{
    struct sockaddr_in addr;
    socklen_t len = 0;
    int res = 0;

    memset(&addr,0,sizeof(addr));
    res = getpeername(fd,(sockaddr*)&addr,&len);
    assert(res != -1);
    return MyAddrInfo(addr);
}
#if 0
std::string MyNet::GetHostName(std::string ipStr)
{
    struct hostent* host = nullptr;
    struct sockaddr_in net4;
    inet_aton(ipStr.c_str(),&net4.sin_addr);
    host = gethostbyaddr(&net4.sin_addr,sizeof(struct in_addr),AF_INET);
    if(host == NULL)
        return "";
    return host->h_name;
}

struct in_addr sock_getip(const char* name)
{
    struct addrinfo hints;
    struct addrinfo* res; /*, *cur;*/
    struct sockaddr_in* addr;
    struct in_addr ret;
    int result;
    ret.s_addr = INADDR_NONE;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family     = AF_INET;
    hints.ai_flags      = AI_PASSIVE;
    hints.ai_protocol   = 0;
    hints.ai_socktype   = SOCK_STREAM;

    result = getaddrinfo(name, NULL, &hints, &res);
    if(result < 0)
    {
        return ret;
    }

#if 0
    for(cur = res; cur != NULL; cur = cur->ai_next)
    {
        addr = (struct sockaddr_in*)cur->ai_addr;
        ret = addr.sin_addr.s_addr;
    }
#endif
    addr = (struct sockaddr_in*)res->ai_addr;
    ret = addr->sin_addr;
    freeaddrinfo(res);
    return ret;
}

//inet_ntop的输入是网络序的*
std::vector<std::string> MyNet::GetHostIpStr(const std::string name)
{
    char** pptr = nullptr;
    struct hostent* host = nullptr;
    char str[INET6_ADDRSTRLEN];
    std::vector<std::string> ips_str;

    ips_str.clear();
    host = gethostbyname(name.c_str());
    if(host == nullptr)
        return ips_str;
#if 0
    printf("official hostname: %s\n",host->h_name);
    for(pptr = host->h_aliases; *pptr != NULL; pptr++)
    {
        printf("alias %s\n",*pptr);
    }
#endif
    switch(host->h_addrtype)
    {
    case AF_INET:
#ifdef AF_INET6
    case AF_INET6:
#endif
        pptr = host->h_addr_list;
        for(;*pptr != NULL; pptr++)
            // [将“点分十进制” －> “整数”]
            ips_str.push_back(inet_ntop(host->h_addrtype,*pptr,str,sizeof(str)));
        break;
    default:
        break;
    }
    return ips_str;
}

//inet_pton的输出是网络序的*
struct in_addr MyNet::GetNetSeriIp(std::string ipStr)
{
    struct in_addr net4;

    inet_pton(AF_INET,ipStr.c_str(),&net4.s_addr);
    return net4;
}
#endif

////////////////////////////////////////////////////////
/// MyAddrInfo
MyAddrInfo::MyAddrInfo()
{
    Init();
}
MyAddrInfo::MyAddrInfo(struct sockaddr_in addr)
{
    Init();
    memcpy(&m_remote_addr,&addr,sizeof(struct sockaddr_in));
}

MyAddrInfo::MyAddrInfo(const std::string ip, const unsigned short port)
{
    Init();
    SetIP(ip);
    SetPort(port);
}

MyAddrInfo::~MyAddrInfo()
{
    if(m_buf != nullptr)
        delete []m_buf;
    m_len = 0;
}

void MyAddrInfo::Init()
{
    m_buf = nullptr;
    m_len = 0;
    memset(&m_remote_addr,0,sizeof(struct sockaddr_in));
    m_remote_addr.sin_family = AF_INET;
}

MyAddrInfo& MyAddrInfo::operator=(MyAddrInfo& other)
{
    if(m_buf != nullptr)
    {
        delete[] m_buf;
        m_buf = nullptr;
    }
    if(other.m_buf != nullptr)
    {
        m_buf = new char[other.m_len];
        memcpy(m_buf,other.m_buf,m_len);
    }

    memcpy(&m_remote_addr,&other.m_remote_addr,sizeof(struct sockaddr_in));
    m_len = other.m_len;
    return *this;
}

bool MyAddrInfo::operator==(const MyAddrInfo& other) const
{
    if(!memcmp(&other.m_remote_addr,&m_remote_addr,sizeof(m_remote_addr)))
        return true;
    return false;
}

int MyAddrInfo::GetData(char** buf)
{
    *buf = m_buf;
    return m_len;
}

void MyAddrInfo::SetData(char* buf, int len)
{
    if(m_buf != nullptr)
        delete[] m_buf;
    m_buf = new char[len];
    memcpy(m_buf,buf,len);
    m_len = len;
}

std::string MyAddrInfo::GetIp()
{
    return MyNet::GetAddrIp(&m_remote_addr);
}

unsigned short MyAddrInfo::GetPort()
{
    return MyNet::GetAddrPort(&m_remote_addr);
}

sockaddr_in MyAddrInfo::GetAddr()
{
    return m_remote_addr;
}

void MyAddrInfo::SetIP(const std::string ip)
{
    if(!ip.empty())
        inet_pton(AF_INET, ip.c_str(), &m_remote_addr.sin_addr);
    else
        m_remote_addr.sin_addr.s_addr = INADDR_ANY;
}

void MyAddrInfo::SetPort(const unsigned short port)
{
    m_remote_addr.sin_port = htons(port);
}

void MyAddrInfo::SetIpAndPort(const std::string ip, const unsigned short port)
{
    SetIP(ip);
    SetPort(port);
}

void MyAddrInfo::SetBoardAddr(const unsigned short port)
{
    m_remote_addr.sin_family=AF_INET;
    m_remote_addr.sin_addr.s_addr=htonl(INADDR_BROADCAST);
    m_remote_addr.sin_port=htons(port);
}


















