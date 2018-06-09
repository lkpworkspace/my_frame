#ifndef __MyWriteable_test_h__
#define __MyWriteable_test_h__
#include "MyFrame.h"
#include "MyTest.h"
#include "MyWriteable.h"

using namespace my_master;

class MyWriteable_test
{
public:
    MyWriteable_test(){

    }

    void test()
    {
        int a = 0;
        void *p = &a;

        MyWriteable w;
        w.AddWriteEvent((myframe::MyEvent*)p);

        std::thread thr([&](){
            while(true)
            {
                sleep(1);
                sem_t* temp = w.SemFind((myframe::MyEvent*)p);
                sem_post(temp);
            }
        });
        thr.detach();

        while(true)
        {
            w.WaitWriteable((myframe::MyEvent*)p);
            printf("aaaa\n");
        }
    }
};

#endif
