#include "MyServerShared.h"
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
        MyDebug("get client connect fd : %d, port %u, ip %s\n",
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
    SetLoop(true);

    /**
     * 设置该线程只处理指定自己ID的事件
     */
    SetSpecifledEv(true);
}

MyGameEngineTask::~MyGameEngineTask()
{
}

/**
 * @brief MyGameEngineTask::Update
 * @param evs 从客户端收到的数据
 * 游戏服务器循环
 */
void MyGameEngineTask::Update(MyList *evs)
{
#if 0
    MyDebugPrint("get client msg count %d\n",evs->Count());
    static int counter = 0;
    MyDebug("loop %d\n", counter);
    MyDebugPrint("loop %d\n", counter);
    ++counter;
#endif
    MyGameServer::sInstance->mGmameMsgs.Append(evs);

    MyGameServer::sInstance->Run();
}
//////////////////////////////////////////////////// MyGameServer
MyGameServer* MyGameServer::sInstance = nullptr;
MyGameServer::MyGameServer()
{
    //MyWorld::sInstance->RegisterCreationFunction('PLAN',MyPlaneServer::StaticCreate);
    MyWorld::sInstance->RegisterCreationFunction('TEST',MyGameObjCtrl_test::StaticCreate);

    MyNetworkManagerServer::StaticInit();

    mMsgPool.RegMsg("MSG_GameMsg",MyGameMsg::StaticCreate);
}

bool MyGameServer::StaticInit()
{
    if(sInstance == nullptr)
        sInstance = (MyGameServer*)new MyGameServer();
    return true;
}

int MyGameServer::Go()
{
    //MyHelp::DaemonInit();
    MyApp app(2);

    /// 设置游戏世界的初始化游戏对象
    SetupWorld();

    /// 添加游戏循环线程
    MyGameEngineTask* ges = new MyGameEngineTask();
    mGameEngineTaskId = ges->GetIdentify();
    MyApp::theApp->AddEvent(ges);

    /// 开启游戏服务器TCP监听
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
