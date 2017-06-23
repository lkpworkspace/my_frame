#include <iostream>
#include "MyFrame.h"
#include "MyTcp.h"

using namespace my_master;
using namespace std;

class MyClient : public MyEasyTcpClient
{
public:
    MyClient(std::string ip, uint16_t port)
        :MyEasyTcpClient(ip,port)
    {

    }

    void Frame(const char *buf, int len)
    {
        printf("get frame\n");
    }

};

int main(int argc, char *argv[])
{
    MyApp app{4,1024};

    MyClient *client = new MyClient("127.0.0.1",4399);
    client->Connect();
    app.AddEvent(client);

    return app.Exec();
}
