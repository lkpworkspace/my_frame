#ifndef __MyAPP_test_h__
#define __MyAPP_test_h__
#include "MyFrame.h"
#include "MyTest.h"
#include <thread>





class MyAPP_test
{
public:
    MyAPP_test()
    {

    }

    void test()
    {
        MyApp app{1};
        app.SetQuitFunc(MyTest::QuitFunc);

    #if 0
        // file monitor test
        MyFileEvent* fileEvent = new MyFileEvent();
        MyFileEvent::StartMonitor("filetest.txt");
    #endif



        // tcp test
    //    MyTcpServer *server = new MyTcpServer("",9999);
    //    server->Bind();
    //    server->Listen(10);
    //    app.AddEvent(server);

    //    MyTcpServer *server1 = new MyTcpServer("",8888);
    //    server1->Bind();
    //    server1->Listen(10);
    //    app.AddEvent(server1);

    #if 0 // udp test
    #if 0
        // udp server
        MyUdp *server = new MyUdp("",4399,true);
        server->Bind();
        app.AddEvent(server);
    #else
        // udp client
        MyUdp *client = new MyUdp("",5555,false);
        app.AddEvent(client);
        std::thread thr([&](){
            sleep(1);
            MyAddrInfo info;
            info.SetPort(4399);
            info.SetIP("127.0.0.1");
            char buf[500];
            memset(buf,1,sizeof(buf));
            while(true)
            {
                usleep(1000*1);
                client->Write(info,buf,500);
            }
        });
        thr.detach();
    #endif
    #endif // end udp test

        // mouse test
        //MyMouseEvent* mouse = new MyMouseEvent;
        //app.AddEvent(mouse);

    #if 0 // MyTFTP test
    #if 0 // MyTFTP client
        MyTFTP* tftp = new MyTFTP("",5555,true);
        tftp->SetRootDir("./");
        app.AddEvent(tftp);

        std::thread thr([&](){
            sleep(1);
            MyAddrInfo info;
            info.SetPort(4399);
            info.SetIP("127.0.0.1");
            printf("argc %d\n",argc);
            if(argc >= 3)
            {
                if(!strcmp(argv[1],"get"))
                {
                    // get file
                    tftp->GetFile(info,argv[2]);
                }
                if(!strcmp(argv[1],"post"))
                {
                    // upload file
                    tftp->SendFile(info,argv[2]);
                }
                MyApp::theApp->Quit();
            }
        });
        thr.detach();
    #else // MyTFTP server
        MyTFTP* tftp = new MyTFTP("",4399,true);
        tftp->SetRootDir("./");
        tftp->Bind();
        app.AddEvent(tftp);
    #endif
    #endif // end MyTFTP test

    #if 0  // test MyNormalEvent
        // see MyTimer used MyNormalEvent
    #endif

    #if 0  // test MyApp quit
        std::thread thr([&](){
            char ch;
            sleep(1);
            while((ch = getchar()) == 'q')
            {
                MyApp::theApp->Quit();
                break;
            }
        });
        thr.detach();
    #endif // end MyApp quit test

        // ev procees
    #if 0
        AllEv* widget = new AllEv;
        widget = widget;
    #endif
        app.Exec();
    }

    /// APP timer测试
    void testn()
    {
        MyApp app(4);

        std::thread thr([&](){
            while(MyHelp::getch() != 'q')
            {
                app.DelLater(NULL,100);
            }
        });
        thr.detach();

        app.Exec();
    }
};




#endif
