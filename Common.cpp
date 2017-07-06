#include "Common.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dlfcn.h>

Common::Common()
{

}

Common::~Common()
{

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
    clock_gettime(CLOCK_REALTIME, &ts);

    now = ts.tv_sec * 1000;
    now += ts.tv_nsec / 1000000;
    return now;
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

    if(chdir("/") < 0)
    {
        perror("chdir");
        exit(-1);
    }
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













