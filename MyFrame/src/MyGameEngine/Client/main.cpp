#include "MyGameEngine/Client/MyGameClientShared.h"

#if 1
#define IP "127.0.0.1"
#else
#define IP "101.37.149.18"
#endif


int main()
{
    MyAddrInfo info(IP,4399);
    MyGameClient::StaticInit("json",info);
    static int loop_count = 0;
    while(true)
    {
        MyGameClient::sInstance->Update();
        usleep(1000*1000);
        printf("loop %d\n", loop_count++);
    }
    return 0;
}
