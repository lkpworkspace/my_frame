#include "GServer.h"
#include "GTestFunc.h"

/**
 *      监听客户端的连接的 TCP 服务器
 */
class GTCPServer : public MyTcpServer
{
public:
    GTCPServer(std::string IP, unsigned short port)
        :MyTcpServer(IP,port)
    {}

    virtual ~GTCPServer(){}

    virtual void* CallBackFunc(MyEvent *ev) override
    {
        GTCPServer* serv = (GTCPServer*)ev;
        MyAddrInfo info;
        while(1)
        {
            int fd = serv->Accpet(info);
            if(fd < 0)
                break;
            GClientProxy *recv = new GClientProxy(fd,info.GetAddr());
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
};



/**
    MSG:
        GMsgType::EMSG_TYPE(2byte) + custom msg(nByte)
    多个读线程：
        获得数据，并将数据发送给工作线程
    一个工作线程：
        获得数据，并进行相应的操作，然后获得回复消息
            1. 登录消息
            2. 匹配消息
                //TODO(lkp)...
            3. 场景消息
                //TODO(lkp)...
            4. 退出消息
            5. 。。。
    一个写线程：
        获得回复消息，并将回复数据发送给客户端
 */
int main(int argc, char* argv[])
{
    GGlobalInit();

    // 测试使用
#ifdef GSERVER_TEST
    TestFunc();
    return -1;
#endif

    //MyHelp::DaemonInit();
    MyApp app(1);

    /// 添加写线程
    GReply* reply = new GReply();
    app.AddEvent(reply);

    /// 添加游戏循环线程
    GServer* gserver = new GServer();
    app.AddEvent(gserver);

    /// 开启游戏服务器TCP监听
    GTCPServer* server = new GTCPServer("127.0.0.1",4399);
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
