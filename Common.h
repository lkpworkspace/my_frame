#ifndef __COMMON_H__
#define __COMMON_H__
// c normal
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
// c++ normal
#include <iostream>
#include <string>
// win or linux
#ifdef WIN32
#include <Windows.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <unistd.h>
#endif

// macro define
#define USE_LOG

#ifndef DEBUG_INFO
#define DEBUG_INFO 1
#endif

#ifndef DEBUG_WARNING
#define DEBUG_WARNING 0
#endif

#ifndef DEBUG_ERROR
#define DEBUG_ERROR 1
#endif

// system V (msg key id)
#define MSG_KEY 1024

#define MSG_LEN 1

#define USE_CONFIG 0

class Common
{
public:
    static bool SetNonblock(int fd, bool b);
    static int GetFileLen(const char* path);
    static bool IsFileExist(const char* path);
    static bool IsDirExist(const char* path);
    static char* GetHomeDir();
    static uint64_t GetTimerNow();
private:
    Common();
    ~Common();
};
/*
    open:(flags)
        O_CREAT      创建文件
        O_EXCL       创建文件时，如果文件存在则出错返回
        O_TRUNC      把文件截断成0
        O_RDONLY     只读（互斥）
        O_WRONLY     只写（互斥）
        O_RDWR       读写（互斥）
        O_APPEND     追加，移动文件读写指针到文件末尾
        O_NONBLOCK   非阻塞标志
        O_SYNC       使每次write都等到物理I/O操作完成，包括文件属性的更新

 */

/*
 * //当文件是符号链接是返回符号连接的文件信息
 * //通过文件路径获得文件信息
 * lstat(char* path, xx)
 * //当文件是符号链接时返回引用的文件信息
 * //通过文件路径获得文件信息
 * stat(char* path,xx)
 * // 通过文件描述符获得文件信息
 * fstat(int fd, xx)
 * 返回值：
 *  0：成功返回
 *  -1：有错误产生
先前所描述的st_mode 则定义了下列数种情况：
    S_IFMT   0170000    文件类型的位遮罩
    S_IFSOCK 0140000    scoket
    S_IFLNK 0120000     符号连接
    S_IFREG 0100000     一般文件
    S_IFBLK 0060000     区块装置
    S_IFDIR 0040000     目录
    S_IFCHR 0020000     字符装置
    S_IFIFO 0010000     先进先出

    S_ISUID 04000     文件的(set user-id on execution)位
    S_ISGID 02000     文件的(set group-id on execution)位
    S_ISVTX 01000     文件的sticky位

    S_IRUSR(S_IREAD) 00400     文件所有者具可读取权限
    S_IWUSR(S_IWRITE)00200     文件所有者具可写入权限
    S_IXUSR(S_IEXEC) 00100     文件所有者具可执行权限

    S_IRGRP 00040             用户组具可读取权限
    S_IWGRP 00020             用户组具可写入权限
    S_IXGRP 00010             用户组具可执行权限

    S_IROTH 00004             其他用户具可读取权限
    S_IWOTH 00002             其他用户具可写入权限
    S_IXOTH 00001             其他用户具可执行权限

上述的文件类型在POSIX中定义了检查这些类型的宏定义：
    S_ISLNK (st_mode)    判断是否为符号连接
    S_ISREG (st_mode)    是否为一般文件
    S_ISDIR (st_mode)    是否为目录
    S_ISCHR (st_mode)    是否为字符装置文件
    S_ISBLK (s3e)        是否为先进先出
    S_ISSOCK (st_mode)   是否为socket
 */

/*
    int fcntl(int fd, int cmd);
    int fcntl(int fd, int cmd, long arg);
    int fcntl(int fd, int cmd, struct flock *lock);
    参数：
    fd：文件描述词。
    cmd：操作命令。
    arg：供命令使用的参数。
    lock：同上

    cmd:
    F_DUPFD    ：复制文件描述词 。
    FD_CLOEXEC ：设置close-on-exec标志。
                如果FD_CLOEXEC位是0，执行execve的过程中，文件保持打开。反之则关闭。
    F_GETFD    ：读取文件描述词标志。
    F_SETFD    ：设置文件描述词标志。
    F_GETFL    ：读取文件状态标志。
    F_SETFL    ：设置文件状态标志。
        其中O_RDONLY， O_WRONLY， O_RDWR， O_CREAT，  O_EXCL， O_NOCTTY 和 O_TRUNC不受影响，
        可以更改的标志有 O_APPEND，O_ASYNC， O_DIRECT， O_NOATIME 和 O_NONBLOCK。
    返回说明：
    成功执行时，对于不同的操作，有不同的返回值
    F_DUPFD： 新文件描述词
    F_GETFD：  标志值
    F_GETFL：  标志值
    F_GETOWN： 文件描述词属主
    F_GETSIG： 读写变得可行时将要发送的通告信号，或者0对于传统的SIGIO行为
    对于其它命令返回0。
    失败返回-1，errno被设为以下的某个值
    EACCES/EAGAIN: 操作不被允许，尚未可行
    EBADF: 文件描述词无效
    EDEADLK: 探测到可能会发生死锁
    EFAULT: 锁操作发生在可访问的地址空间外
    EINTR: 操作被信号中断
    EINVAL： 参数无效
    EMFILE: 进程已超出文件的最大可使用范围
    ENOLCK: 锁已被用尽

fcntl功能：
    • 复制一个现存的描述符(cmd=F_DUPFD)
    • 获得/设置文件描述符标记(cmd=F_GETFD或F_SETFD)
    • 获得/设置文件状态标志(cmd=F_GETFL或F_SETFL)
    • 获得/设置异步I/O有权(cmd=F_GETOWN或F_SETOWN)
    • 获得/设置记录锁(cmd=F_GETLK,F_SETLK或F_SETLKW)

 */

#endif // COMMON_H
