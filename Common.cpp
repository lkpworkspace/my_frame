#include "Common.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

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
