#ifndef __MyTimer_test_h__
#define __MyTimer_test_h__
#include "MyFrame.h"
#include "MyTest.h"

class MyTimer_test
{
public:
    MyTimer_test()
    {

    }

    void test()
    {
        MyApp app{1};
        app.SetQuitFunc(MyTest::QuitFunc);

        // timer test
        std::thread thr([&](){
            sleep(1);
            MyTimer* timer = new MyTimer(500);
            timer->SetCallFunc(&MyTest::TimerFunc,NULL);
            timer->Start();
            sleep(5);
            timer->Stop();
            sleep(1);
            printf("second\n");
            timer->Start();
        });
        thr.detach();


        app.Exec();
    }
};




#endif
