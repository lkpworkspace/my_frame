#include "MyCommon.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

Common::Common()
{

}

Common::~Common()
{

}

const char Common::getch()
{
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

bool Common::SetNonblock(int fd, bool b)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if(b)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags) != -1;
}

int Common::GetFileLen(const char* path)
{
    struct stat st;
    int ret = lstat(path, &st);
    if(ret < 0)
        return ret;

    if(S_ISREG(st.st_mode))
        return st.st_size;
    return 0;
}

bool Common::IsFileExist(const char* path)
{
    struct stat st;
    int ret = lstat(path, &st);
    if(ret < 0)
        return false;

    if(S_ISREG(st.st_mode))
        return true;
    else
        return false;
}

bool Common::IsDirExist(const char* path)
{
    struct stat st;
    int ret = lstat(path, &st);
    if(ret < 0)
        return 0;

    if(S_ISDIR(st.st_mode))
        return true;
    else
        return false;
}

char* Common::GetHomeDir()
{
    return getenv("HOME");
}

uint64_t Common::GetTimerNow()
{
    uint64_t now;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    now = ts.tv_sec * 1000u;
    now += ts.tv_nsec / 1000000u;
    return now;
}

int Common::BytesAvailable(int fd)
{
    int len = 0, res = -1;
    res = ioctl(fd,FIONREAD,&len);
    return len;
}

void Common::DaemonInit()
{
    pid_t pid;
    if((pid = fork()) < 0)
    {
            perror("fork");
            exit(-1);
    }else if(pid != 0)
        exit(0);
    setsid();

//    if(chdir("/") < 0)
//    {
//        perror("chdir");
//        exit(-1);
//    }
    umask(0);
    close(0);
    open("/dev/null",O_RDWR);
    dup2(0,1);
    dup2(0,2);
}


/*
#include <dlfcn.h>
函数定义：　void * dlopen( const char * pathname, int mode);
函数描述：　在dlopen（）函数以指定模式打开指定的动态连接库文件，并返回一个句柄给调用进程。使用dlclose（）来卸载打开的库。
    mode：分为这两种RTLD_LAZY 暂缓决定，等有需要时再解出符号;
    RTLD_NOW 立即决定，返回前解除所有未决定的符号。
    RTLD_LOCAL 与RTLD_GLOBAL作用相反，动态库中定义的符号不能被其后打开的其它库重定位。
    如果没有指明是RTLD_GLOBAL还是RTLD_LOCAL，则缺省为RTLD_LOCAL。
    RTLD_GLOBAL 动态库中定义的符号可被其后打开的其它库重定位。
    RTLD_GROUPRTLD_WORLD
返回值:　打开错误返回NULL，成功，返回库引用。
编译时候要加入 -ldl (指定dl库)：例如　gcc test.c -o test -ldl

如果一个函数库里面有一个输出的函数名字为_init,那么_init就会在dlopen（）这个函数返回前被执行。
我们可以利用这个函数在我的函数库里面做一些初始化的工作。我们后面会继续讨论这个问题的。
*/
void* Common::DllOpen(const char * pathname, int mode)
{
    // RTLD_NOW(use this)
    // RTLD_LAZY
    //
    void* handle;
    void* error;

    handle = dlopen(pathname,mode);
    error = dlerror();
    if(error)
    {
        printf("Open Dll %s failed, %s\n",pathname, (char*)error);
        return NULL;
    }
    return handle;
}

void* Common::DllUse(void *handle, const char *symbol)
{
    void* func;
    void* error;

    func = dlsym(handle, symbol);
    error = dlerror();
    if(error)
    {
        printf("Use DLL sym failed, %s\n", (char*)error);
        return NULL;
    }
    return func;
}

int Common::DllClose(void *handle)
{
    int res;
    void* error;
    res = dlclose(handle);
    if(res != 0)
    {
        error = dlerror();
        if(error)
        {
            printf("DLL close failed, %s\n", (char*)error);
            return -1;
        }
    }
    return res;
}


////////////////////////////////////////////////////////////
/// MySelfProtocol
char *MySelfProtocol::GetBuf(int *len)
{
    if(len != NULL)
        *len = MYPROTO_MAX_BUF_SIZE;
    return (char*)malloc(MYPROTO_MAX_BUF_SIZE);
}

void MySelfProtocol::FreeBuf(char* buf)
{
    if(buf != NULL)
        free(buf);
}

////////////////////////////////////////////////
/// handle
uint16_t MySelfProtocol::HandleHeader(const char *buf)
{
    uint16_t head;
    memcpy(&head,&buf[0],sizeof(uint16_t));
    return head;
}

uint16_t MySelfProtocol::HandleShort(int offset, const char* buf, int len)
{
    if(offset + sizeof(uint16_t) > len)
        return 0;
    uint16_t data_len = 0;
    memcpy(&data_len,&buf[offset],sizeof(data_len));
    return data_len;
}

uint8_t MySelfProtocol::HandleChar(int offset, const char *buf, int len)
{
    if(offset + sizeof(uint8_t) > len)
        return 0;
    uint8_t data_len = 0;
    memcpy(&data_len,&buf[offset],sizeof(uint8_t));
    return data_len;
}

std::string MySelfProtocol::HandleString(int offset, const char *buf, int len)
{
    char temp_buf[MYPROTO_MAX_BUF_SIZE] = {0};
    int str_len = strlen(&buf[offset]);
    memcpy(temp_buf,&buf[offset],str_len);
    return std::string(temp_buf);
}

data_t MySelfProtocol::HandleData(int offset, const char* buf, int len)
{
    data_t my_data;

    my_data.len = HandleShort(offset,buf,len);
    memcpy(my_data.buf,&buf[offset + sizeof(uint16_t)],my_data.len);
    return my_data;
}

////////////////////////////////////////////////
/// build
int MySelfProtocol::BuildHeader(uint16_t head, char* buf, int len)
{
    memset(buf,0,len);
    memcpy(buf,&head,sizeof(head));
    return sizeof(head);
}

int MySelfProtocol::BuildShort(uint16_t datalen, int offset, char* buf, int len)
{
    if(offset + sizeof(datalen) > len)
        return 0;
    memcpy(&buf[offset],&datalen,sizeof(datalen));
    return sizeof(datalen);
}

int MySelfProtocol::BuildChar(uint8_t ch,int offset, char* buf, int len)
{
    if(offset + sizeof(ch) > len)
        return 0;
    memcpy(&buf[offset],&ch,sizeof(ch));
    return sizeof(ch);
}

int MySelfProtocol::BuildString(const char* str, int offset, char* buf, int len)
{
    int str_len = strlen(str);
    if(str_len + 1 + offset > len)
        return 0;
    memcpy(&buf[offset],str,str_len);
    return (str_len + 1);
}

int MySelfProtocol::BuildData(const char* data, uint16_t data_len, int offset, char* buf, int len)
{
    int len_offset = BuildShort(data_len,offset,buf,len);
    if(data_len + offset + len_offset > len)
        return 0;
    memcpy(&buf[offset + len_offset],data,data_len);
    return (data_len + len_offset);
}










