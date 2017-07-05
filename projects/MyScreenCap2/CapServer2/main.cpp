#include <iostream>
#include "MyFrame.h"
#include "MyTcp.h"
#include "MyServer.h"
#include "MyUdp.h"
#include <thread>

using namespace my_master;
using namespace std;

#if 0
class MyClient : public MyEasyTcpClient
{
public:
    MyClient(std::string ip, uint16_t port)
        :MyEasyTcpClient(ip,port)
    {

    }

    int Frame(const char *buf, int len)
    {
        printf("client frame get buf\n");
        return true;
    }
};
#endif

int main(int argc, char *argv[])
{
    UNUSE_ARG(argc,argv);
    MyApp app(4,1024);

    MyTcpServer *server = new MyTcpServer("",4399);
    server->SetReuseSock();
    server->Bind();
    server->Listen();
    app.AddEvent(server);

    // test server
#if 0
    std::thread thr([&](){
        sleep(2);
        MyClient *client = new MyClient("127.0.0.1",4399);
        client->Connect();
        int len = 0;
        char* buf = MySelfProtocol::GetBuf(&len);
        int index = MySelfProtocol::BuildString("something",0,buf,len);
        for(int i = 0; i < 100; ++i)
            client->EasyWrite(buf,index);
        sleep(2);
        MySelfProtocol::FreeBuf(buf);
        client->Close();
        sleep(2);
    });
    thr.detach();
#endif

    MyServer* allevent = new MyServer;
    allevent = allevent;

    return app.Exec();
}
