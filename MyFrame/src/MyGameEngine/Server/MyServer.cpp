#include "Server/MyServerShared.h"
#include "MyFrame.h"

#include <thread>

//////////////////////////////////////////////////// MyGameTcpServer
MyGameTcpServer::MyGameTcpServer()
    :MyTcpServer(IP,PORT)
{
}

MyGameTcpServer::~MyGameTcpServer()
{
}

void* MyGameTcpServer::CallBackFunc(MyEvent *ev)
{
    MyGameTcpServer* serv = (MyGameTcpServer*)ev;
    MyAddrInfo info;
    while(1)
    {
        int fd = serv->Accpet(info);
        if(fd < 0)
            break;
        MyClientProxy *recv = new MyClientProxy(fd,info.GetAddr());
        MyDebugPrint("get client connect fd : %d, port %u, ip %s\n",
               fd,
               info.GetPort(),
               info.GetIp().c_str());

        MyApp::theApp->AddEvent(recv);
    }
    MyApp::theApp->AddEvent(ev);
    return NULL;
}

//////////////////////////////////////////////////// MyGameEngineTask
MyGameEngineTask::MyGameEngineTask()
{
    /**
     * 设置线程是帧循环的，而不是基于事件驱动的
     */
    // 调试阶段不使用帧循环驱动，而是用事件驱动(default)
    //SetLoop(true);

    /**
     * 设置该线程只处理指定自己ID的事件
     */
    SetSpecifledEv(true);
}

MyGameEngineTask::~MyGameEngineTask()
{
}

void MyGameEngineTask::Update(MyList *evs)
{
    MyGameServer::sInstance->mGmameMsgs.Append(evs);

#if 1
    static int counter = 0;
    MyDebug("loop %d\n", counter);
    MyDebugPrint("loop %d\n", counter);
    ++counter;
#endif

    MyGameServer::sInstance->Run();

//    usleep(1000*1000);
}
//////////////////////////////////////////////////// MyGameServer
MyGameServer* MyGameServer::sInstance = nullptr;

MyGameServer::MyGameServer()
{
    //TODO(lkp): server init ...
    MyNetworkManagerServer::StaticInit();

}

bool MyGameServer::StaticInit()
{
    if(sInstance == nullptr)
        sInstance = new MyGameServer();
    return true;
}

int MyGameServer::Go()
{
    SetupWorld();

    //MyHelp::DaemonInit();
    MyApp app(2);

    // process task
    MyGameEngineTask* ges = new MyGameEngineTask();
    mGameEngineTaskId = ges->GetIdentify();
    MyApp::theApp->AddEvent(ges);

    // tcp server
    MyGameTcpServer* server = new MyGameTcpServer();
    server->SetReuseSock();
    server->Bind();
    server->Listen();
    server->SetNonblock(true);
    app.AddEvent(server);

//    std::thread thr([&](){
//        while(MyHelp::getch() == 'q')
//        {
//            app.Quit();
//            MyDebugPrint("thread print 'q'\n");
//        }
//        MyDebugPrint("thread print key\n");
//    });
//    thr.detach();

    return app.Exec();
}

/* 1 */
int MyGameServer::Run()
{
    return MyEngine::Run();
}

/* 3 */
void MyGameServer::DoFrame()
{
    // process msg
    if(!mGmameMsgs.IsEmpty())
    {
        MyNetworkManagerServer::sInstance->ProcessIncomingPackets(&mGmameMsgs);
    }

    // update world
    MyEngine::DoFrame();

    // send updated msg
    MyNetworkManagerServer::sInstance->SendOutgoingPackets();
}

void MyGameServer::SpawnPlaneForPlayer(int inPlayerId)
{

}

void MyGameServer::SetupWorld()
{

}
