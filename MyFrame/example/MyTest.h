#ifndef __MyTest_h__
#define __MyTest_h__

class MyTest
{
public:
    static void* QuitFunc(void*)
    {
        printf("ok, this app was exit\n");
        return nullptr;
    }

    static void* TimerFunc(void*)
    {
        printf("timer\n");
        return nullptr;
    }
};

#endif
