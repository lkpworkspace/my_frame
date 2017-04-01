#include "MyNet.h"
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
{

}

MyNet::~MyNet()
{

}


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




















