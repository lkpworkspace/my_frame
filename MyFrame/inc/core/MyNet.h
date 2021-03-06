#ifndef __MYNET_H__
#define __MYNET_H__
#include <string>
#include <vector>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "Windows.h"
#include "WinSock2.h"
#include "Ws2tcpip.h"
typedef int socklen_t;
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/tcp.h>
#include <netdb.h>
typedef int SOCKET;
#endif


// only support IPv4
class MyAddrInfo;
class MyNet
{
public:
    MyNet();
    ~MyNet();
    static int GetLocalIp(const char* eth, char* ip);
    // useful
    static std::string GetAddrIp(sockaddr_in* addr);
    static unsigned short GetAddrPort(sockaddr_in* addr);
    /*
    补充：getsockname和getpeername调度时机很重要，如果调用时机不对，则无法正确获得地址和端口。
    TCP
    对于服务器来说，在bind以后就可以调用getsockname来获取本地地址和端口，虽然这没有什么太多的意义。
    getpeername只有在链接建立以后才调用，否则不能正确获得对方地址和端口，所以他的参数描述字一般是链接描述字而非监听套接口描述字。
    对于客户端来说，在调用socket时候内核还不会分配IP和端口，此时调用getsockname不会获得正确的端口和地址
    (当然链接没建立更不可能调用getpeername)，
    当然如果调用了bind 以后可以使用getsockname。想要正确的到对方地址（一般客户端不需要这个功能），
    则必须在链接建立以后，同样链接建立以后，此时客户端地址和端口就已经被指定，此时是调用getsockname的时机。
    UDP
    UDP分为链接和没有链接2种(这个到UDP与connect可以找到相关内容）
    没有链接的UDP不能调用getpeername，但是可以调用getsockname,和TCP一样，他的地址和端口不是在调用socket就指定了，
    而是在第一次调用sendto函数以后
    已经链接的UDP，在调用connect以后，这2个函数都是可以用的
    (同样，getpeername也没太大意义。如果你不知道对方的地址和端口，不可能会调用connect)。
   */
    static MyAddrInfo GetSockInfo(const int fd);
    static MyAddrInfo GetRemoteSockInfo(const int fd);

    static int SetReuseSock(int fd);
#if 0
    // get host info
    // method1
    static std::string GetHostName(std::string ipStr);
    static std::vector<std::string> GetHostIpStr(const std::string name); // ok
    // method2
    static struct in_addr sock_getip(const char* name);

    // can not use this function
    // get server info
    // getservbyname
    // getservbyport

    // get host name
    // uanme

    // getsocketname func
    static struct in_addr GetNetSeriIp(std::string ipStr);
#endif
private:
};

class MyAddrInfo
{
    friend class MyUdp;
public:
    MyAddrInfo();
    MyAddrInfo(struct sockaddr_in addr);
    MyAddrInfo(const std::string ip, const unsigned short port);
    ~MyAddrInfo();

    void Init();
    //////////////////////////////////////////////////
    /// get method
    std::string GetIp();
    unsigned short GetPort();
    sockaddr_in& GetAddr();
    //////////////////////////////////////////////////
    /// set method
    void SetIP(const std::string ip);
    void SetPort(const unsigned short port);
    void SetIpAndPort(const std::string ip, const unsigned short port);
    void SetBoardAddr(const unsigned short port);
    void SetAddr(const struct sockaddr_in& addr);

    MyAddrInfo& operator=(MyAddrInfo& other);
    bool operator==(const MyAddrInfo& other) const;
private:
    struct sockaddr_in m_remote_addr;      // remote udp sockaddr
};






/*
 *#include <arpa/inet.h>
 * // 本地序列转换网络字节序列
 * uint32_t htonl(uint32_t hostlong);
 * uint16_t htons(uint16_t hostshort);
 * // 网络字节序列转换本地字节序列
 * uint32_t ntohl(uint32_t netlong);
 * uint16_t ntohs(uint16_t netshort);
 *注：
 *  n: network
 *  h: host
 *  l: long
 *  s: short
 *
 * // [将 字符串 －> 整数 ]转换字符串到网络地址
 * int inet_pton(int af,const char* cp, void* buf);
 * // [将 整数 －> 字符串 ]转换网络二进制结构到ASCII类型的地址
 * const char* inet_ntop(int af, const char* cp,char* buf, socklen_t len);
 *注：
 *  p: presentation
 *  n: numeric
 */

/*
 *  raw socket coding function
 *  设置混杂模式：ifconfig eth0 promisc
 *  取消混杂模式：ifconfig eth0 -promisc
int socket ( int family, int type, int protocol );
参数：
    family：协议族 这里写 PF_PACKET
    type：  套接字类，这里写 SOCK_RAW
    protocol：协议类别，指定可以接收或发送的数据包类型，不能写 “0”，取值如下，注意，传参时需要用 htons() 进行字节序转换。
        ETH_P_IP：IPV4数据包
        ETH_P_ARP：ARP数据包
        ETH_P_ALL：任何协议类型的数据包
返回值：
    成功( >0 )：套接字，这里为链路层的套接字
    失败( <0 )：出错

ssize_t recvfrom(  int sockfd,
            void *buf,
            size_t nbytes,
            int flags,
            struct sockaddr *from,
            socklen_t *addrlen );
参数：
    sockfd:原始套接字
    buf：接收数据缓冲区
    nbytes:接收数据缓冲区的大小
    flags：套接字标志(常为0)
    from：这里没有用，写 NULL
    addrlen：这里没有用，写 NULL
返回值：
    成功：接收到的字符数
    失败：-1

struct ifreq req;   //网络接口地址
strncpy(req.ifr_name, "eth0", IFNAMSIZ);            //指定网卡名称
if(-1 == ioctl(sock_raw_fd, SIOCGIFINDEX, &req))    //获取网络接口
{
    perror("ioctl");
    close(sock_raw_fd);
    exit(-1);
}

req.ifr_flags |= IFF_PROMISC;
if(-1 == ioctl(sock_raw_fd, SIOCSIFINDEX, &req))    //网卡设置混杂模式
{
    perror("ioctl");
    close(sock_raw_fd);
    exit(-1);
}
*/

/*domin:
 *  AF_INET
 *  AF_INET6
 *  AF_UNIX
 *
 *type:
 *  SOCK_STREAM
 *  SOCK_DGRAM
 *
 *  SOCK_RAW
 *  SOCK_SEQPACKET
 *protocol:
 *  0     默认协议
 *
 */

/*
    #include <sys/types.h>
    #include <sys/socket.h>

    int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);
    int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen);
    参数：
    sock：将要被设置或者获取选项的套接字。
    level：选项所在的协议层。
    optname：需要访问的选项名。
    optval：对于getsockopt()，指向返回选项值的缓冲。对于setsockopt()，指向包含新选项值的缓冲。
    optlen：对于getsockopt()，作为入口参数时，选项值的最大长度。作为出口参数时，选项值的实际长度。对于setsockopt()，现选项的长度。


    返回说明：
    成功执行时，返回0。失败返回-1，errno被设为以下的某个值
    EBADF：sock不是有效的文件描述词
    EFAULT：optval指向的内存并非有效的进程空间
    EINVAL：在调用setsockopt()时，optlen无效
    ENOPROTOOPT：指定的协议层不能识别选项
    ENOTSOCK：sock描述的不是套接字

    参数详细说明：
    level：
    SOL_SOCKET:通用套接字选项.
    IPPROTO_IP:IP选项.
    IPPROTO_TCP:TCP选项.
    选项名称　　　　　　　　说明　　　　　　　　　　　　　　　　　　数据类型
    ========================================================================
                SOL_SOCKET
    ------------------------------------------------------------------------
    SO_BROADCAST　　　　　　允许发送广播数据　　　　　　　　　　　　int
    SO_DEBUG　　　　　　　　允许调试　　　　　　　　　　　　　　　　int
    SO_DONTROUTE　　　　　　不查找路由　　　　　　　　　　　　　　　int
    SO_ERROR　　　　　　　　获得套接字错误　　　　　　　　　　　　　int
    SO_KEEPALIVE　　　　　　保持连接　　　　　　　　　　　　　　　　int
    SO_LINGER　　　　　　　 延迟关闭连接　　　　　　　　　　　　　　struct linger
    SO_OOBINLINE　　　　　　带外数据放入正常数据流　　　　　　　　　int
    SO_RCVBUF　　　　　　　 接收缓冲区大小　　　　　　　　　　　　　int
    SO_SNDBUF　　　　　　　 发送缓冲区大小　　　　　　　　　　　　　int
    SO_RCVLOWAT　　　　　　 接收缓冲区下限　　　　　　　　　　　　　int
    SO_SNDLOWAT　　　　　　 发送缓冲区下限　　　　　　　　　　　　　int
    SO_RCVTIMEO　　　　　　 接收超时　　　　　　　　　　　　　　　　struct timeval
    SO_SNDTIMEO　　　　　　 发送超时　　　　　　　　　　　　　　　　struct timeval
    SO_REUSERADDR　　　　　 允许重用本地地址和端口　　　　　　　　　int
    SO_TYPE　　　　　　　　 获得套接字类型　　　　　　　　　　　　　int
    SO_BSDCOMPAT　　　　　　与BSD系统兼容　　　　　　　　　　　　　 int
    ========================================================================
                IPPROTO_IP
    ------------------------------------------------------------------------
    IP_HDRINCL　　　　　　　在数据包中包含IP首部　　　　　　　　　　int
    IP_OPTINOS　　　　　　　IP首部选项　　　　　　　　　　　　　　　int
    IP_TOS　　　　　　　　　服务类型
    IP_TTL　　　　　　　　　生存时间　　　　　　　　　　　　　　　　int
    ========================================================================
                IPPRO_TCP
    ------------------------------------------------------------------------
    TCP_MAXSEG　　　　　　　TCP最大数据段的大小　　　　　　　　　　 int
    TCP_NODELAY　　　　　　 不使用Nagle算法　　　　　　　　　　　　 int
    示例：
    // 接收缓冲区
    int nRecvBuf=32*1024;         //设置为32K
    setsockopt(s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));

    //发送缓冲区
    int nSendBuf=32*1024;//设置为32K
    setsockopt(s,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
 注意：
    当设置TCP套接口接收缓冲区的大小时，函数调用顺序是很重要的，
    因为TCP的窗口规模选项是在建立连接时用SYN与对方互换得到的。
    对于客户，O_RCVBUF选项必须在connect之前设置；对于服务器，SO_RCVBUF选项必须在listen前设置。

 结合原理说明：
        1.每个套接口都有一个发送缓冲区和一个接收缓冲区。 接收缓冲区被TCP和UDP用来将接收到的数据一直保存到由应用进程来读。
        TCP：TCP通告另一端的窗口大小。 TCP套接口接收缓冲区不可能溢出，因为对方不允许发出超过所通告窗口大小的数据。
        这就是TCP的流量控制，如果对方无视窗口大小而发出了超过窗口大小的数据，则接 收方TCP将丢弃它。
        UDP：当接收到的数据报装不进套接口接收缓冲区时，此数据报就被丢弃。UDP是没有流量控制的；
        快的发送者可以很容易地就淹没慢的接收者，导致接收方的UDP丢弃数据报。
        2.我们经常听说tcp协议的三次握手,但三次握手到底是什么，其细节是什么，为什么要这么做呢?
        第一次:客户端发送连接请求给服务器，服务器接收;
        第二次:服务器返回给客户端一个确认码,附带一个从服务器到客户端的连接请求,客户机接收,确认客户端到服务器的连接.
        第三次:客户机返回服务器上次发送请求的确认码,服务器接收,确认服务器到客户端的连接.
        我们可以看到:
        1. tcp的每个连接都需要确认.
        2. 客户端到服务器和服务器到客户端的连接是独立的.
        我们再想想tcp协议的特点:连接的,可靠的,全双工的,实际上tcp的三次握手正是为了保证这些特性的实现.

        3.setsockopt的用法

        1.closesocket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket：
        BOOL bReuseaddr=TRUE;
        setsockopt(s,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(BOOL));

        2. 如果要已经处于连接状态的soket在调用closesocket后强制关闭，不经历TIME_WAIT的过程：
        BOOL bDontLinger = FALSE;
        setsockopt(s,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(BOOL));

        3.在send(),recv()过程中有时由于网络状况等原因，发收不能预期进行,而设置收发时限：
        int nNetTimeout=1000;//1秒
        //发送时限
        setsockopt(socket，SOL_S0CKET,SO_SNDTIMEO，(char *)&nNetTimeout,sizeof(int));
        //接收时限
        setsockopt(socket，SOL_S0CKET,SO_RCVTIMEO，(char *)&nNetTimeout,sizeof(int));

        4.在send()的时候，返回的是实际发送出去的字节(同步)或发送到socket缓冲区的字节
        (异步);系统默认的状态发送和接收一次为8688字节(约为8.5K)；在实际的过程中发送数据
        和接收数据量比较大，可以设置socket缓冲区，而避免了send(),recv()不断的循环收发：
        // 接收缓冲区
        int nRecvBuf=32*1024;//设置为32K
        setsockopt(s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
        //发送缓冲区
        int nSendBuf=32*1024;//设置为32K
        setsockopt(s,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));

        5. 如果在发送数据的时，希望不经历由系统缓冲区到socket缓冲区的拷贝而影响
        程序的性能：
        int nZero=0;
        setsockopt(socket，SOL_S0CKET,SO_SNDBUF，(char *)&nZero,sizeof(nZero));

        6.同上在recv()完成上述功能(默认情况是将socket缓冲区的内容拷贝到系统缓冲区)：
        int nZero=0;
        setsockopt(socket，SOL_S0CKET,SO_RCVBUF，(char *)&nZero,sizeof(int));

        7.一般在发送UDP数据报的时候，希望该socket发送的数据具有广播特性：
        BOOL bBroadcast=TRUE;
        setsockopt(s,SOL_SOCKET,SO_BROADCAST,(const char*)&bBroadcast,sizeof(BOOL));

        8.在client连接服务器过程中，如果处于非阻塞模式下的socket在connect()的过程中可以设置connect()延时,
        直到accpet()被呼叫(本函数设置只有在非阻塞的过程中有显著的作用，在阻塞的函数调用中作用不大)
        BOOL bConditionalAccept=TRUE;
        setsockopt(s,SOL_SOCKET,SO_CONDITIONAL_ACCEPT,(const char*)&bConditionalAccept,sizeof(BOOL));

        9.如果在发送数据的过程中(send()没有完成，还有数据没发送)而调用了closesocket(),
        以前我们一般采取的措施是"从容关闭"shutdown(s,SD_BOTH),但是数据是肯定丢失了，
        如何设置让程序满足具体应用的要求(即让没发完的数据发送出去后在关闭socket)？
        struct linger {
        u_short l_onoff;
        u_short l_linger;
        };
        linger m_sLinger;
        m_sLinger.l_onoff=1;//(在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
        // 如果m_sLinger.l_onoff=0;则功能和2.)作用相同;
        m_sLinger.l_linger=5;//(容许逗留的时间为5秒)
        setsockopt(s,SOL_SOCKET,SO_LINGER,(const char*)&m_sLinger,sizeof(linger));
 */


//    struct in_addr
//    {
//
//    };

//    struct addrinfo {
//         int ai_flags;  /* customize behavior */
//         int ai_family; /* address family */
//         int ai_socktype; /* socket type */
//         int ai_protocol; /* protocol */
//         socklen_t ai_addrlen; /* length in bytes of address */
//         struct sockaddr *ai_addr; /* address */
//         char *ai_canonname; /* canonical name of host */
//         struct addrinfo *ai_next; /* next in list */
//         .
//         .
//         .
//    };
//    ai_family指定了地址族，可取值如下：
//    AF_INET          2            IPv4
//    AF_INET6        23            IPv6
//    AF_UNSPEC        0            协议无关
//    ai_socktype指定套接字的类型:
//    SOCK_STREAM        1            流
//    SOCK_DGRAM        2            数据报
//
//    int getaddrinfo(const char *node,
//                    const char *service,
//                    const struct addrinfo *hints,
//                    struct addrinfo **res);
//    void freeaddrinfo(struct addrinfo *res);
//    struct sockaddr_in:

#endif // MYNET_H
