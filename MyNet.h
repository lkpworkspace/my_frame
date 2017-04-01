#ifndef __MYNET_H__
#define __MYNET_H__
#include "Common.h"
#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <netdb.h>
#endif

namespace my_master {
// only support IPv4
class MyNet
{
public:
    MyNet();
    ~MyNet();
    // get host info
    // method1
    static std::string GetHostName(std::string ipStr);
    static std::vector<std::string> GetHostIpStr(const std::string name); // ok
    // method2
    static struct in_addr sock_getip(const char* name);

    // get server info
    // getservbyname
    // getservbyport

    // get host name
    // uanme

    // getsocketname func
    //
    static struct in_addr GetNetSeriIp(std::string ipStr);
private:
};

} // end namespace
#endif // MYNET_H
