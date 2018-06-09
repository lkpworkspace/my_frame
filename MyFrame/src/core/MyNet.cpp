#include "MyNet.h"
#include <assert.h>


#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
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

int MyNet::GetLocalIp(const char* eth, char* ip)
{
    struct sockaddr_in *addr;
    struct ifreq req;
    int fd;

    memset(&req, 0, sizeof(req));
    fd = socket(AF_INET, SOCK_STREAM, 0);
    strcpy(req.ifr_name, eth);
    ioctl(fd, SIOCGIFADDR, &req);
    addr = (struct sockaddr_in*)&req.ifr_addr;
    strcpy(ip,inet_ntoa(addr->sin_addr));
    close(fd);
    return 0;
}

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

int MyNet::SetReuseSock(int fd)
{
    int flag = 1,len = sizeof(int);
    char flag_ch;
#ifdef linux
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, len) == -1)
#else
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag_ch, len) == -1)
#endif
    {
        perror("setsockopt");
        return -1;
    }
    return 0;
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
{}

void MyAddrInfo::Init()
{
    memset(&m_remote_addr,0,sizeof(struct sockaddr_in));
    m_remote_addr.sin_family = AF_INET;
}

MyAddrInfo& MyAddrInfo::operator=(MyAddrInfo& other)
{
    memcpy(&m_remote_addr,&other.m_remote_addr,sizeof(struct sockaddr_in));
    return *this;
}

bool MyAddrInfo::operator==(const MyAddrInfo& other) const
{
    if(!memcmp(&other.m_remote_addr,&m_remote_addr,sizeof(m_remote_addr)))
        return true;
    return false;
}

std::string MyAddrInfo::GetIp()
{
    char ip_buf[64] = {0};
    inet_ntop(AF_INET,&(m_remote_addr.sin_addr),ip_buf,sizeof(ip_buf));
    return std::string(ip_buf);
}

unsigned short MyAddrInfo::GetPort()
{
    return ntohs(m_remote_addr.sin_port);
}

sockaddr_in &MyAddrInfo::GetAddr()
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

void MyAddrInfo::SetAddr(const struct sockaddr_in& addr)
{
    memcpy(&m_remote_addr,&addr,sizeof(struct sockaddr_in));
}


















