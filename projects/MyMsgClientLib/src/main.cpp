#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <termios.h>
#include <string.h>
#include <thread>
#include "MyFrame.h"
#include "MyMsg.h"
#include "MyTcp.h"
#if 1
#define IP "101.37.149.18"
#else
#define IP "127.0.0.1"
#endif
#define PORT 4399
#define PASS "123456"
#if 0
#define SRC "123456789"
#define DEST "987654321"
#else
#define DEST "123456789"
#define SRC "987654321"
#endif
#define MSG  "hello,world"

#define FOREACH(buf,len) do{ \
        for(int i = 0; i < len; ++i) \
        { \
            printf("%02d\t",(unsigned char)buf[i]); \
        } \
        printf("\n"); \
    }while(0)

class MyMsgClient : public my_master::MyEasyTcpClient
{
public:
    MyMsgClient(std::string ip, uint16_t port)
        :MyEasyTcpClient(ip,port)
    {

    }

    virtual ~MyMsgClient()
    {

    }

    virtual int Frame(const char *buf, int len)
    {
        if(len == 0)
        {
            MyDebugPrint("server close connect\n");
            MyApp::theApp->DelLater(this,3000);
            return false;
        }
        SingleMsg_t smsg;
        MsgAnswer_t ma;
        switch(MyMsgHandle(buf))
        {
        case HD_SIGMSG:
            MyMsgHandleSingleMsg(buf,len,&smsg);
            MyDebugPrint("%s\n",smsg.buf);
            break;
        case HD_ANSWER:
            MyMsgHandleAnswer(&ma,buf,len);
            MyDebugPrint("request: %s, answer: %s\n",MyMsgGetRequest(ma.request),MyMsgGetErr(ma.err_code));
            break;
        default:
            break;
        }
        return true;
    }

    virtual std::string GetClassType(){return "MyMsgClient";}
};

const char getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}

int main(int argc, char*argv[])
{
    MyApp app(4);
    MyMsgInit();

    MyMsgClient *client = new MyMsgClient(IP,PORT);
    client->Connect();
    app.AddEvent(client);

    std::thread thr([&](){
        char ch = '\0';
        char* buf = NULL;
        int outlen = 0;
        printf("command usage : \n");
        printf("\tl for login\n");
        printf("\ts for send message\n");
        printf("\tm for send message loop\n");
        printf("\tq for quit\n");

        while((ch = getch()) != 'q')
        {
            switch(ch)
            {
            case 'l':
                buf = MyMsgLogin(SRC,PASS,&outlen);
                MyDebugPrint("%d byte write\n",outlen);
//                for(int i = 0; i < outlen; ++i)
//                    printf("%02d\t",(unsigned char)buf[i]);
//                printf("\n");
                client->EasyWrite(buf,outlen);
                break;
            case 's':
                buf = MyMsgBuildSingleMsg(SRC,DEST,"hello",5,&outlen);
                MyDebugPrint("%d byte write\n",outlen);
//                for(int i = 0; i < outlen; ++i)
//                    printf("%02d\t",(unsigned char)buf[i]);
//                printf("\n");
                client->EasyWrite(buf,outlen);
                break;
            case 'm':
                while(1)
                {
                    buf = MyMsgBuildSingleMsg(SRC,DEST,"hello",5,&outlen);
                    MyDebugPrint("%d byte write\n",outlen);
    //                for(int i = 0; i < outlen; ++i)
    //                    printf("%02d\t",(unsigned char)buf[i]);
    //                printf("\n");
                    client->EasyWrite(buf,outlen);
                    usleep(1000*16);
                }
                break;
            default:
                break;
            }
            printf("command usage : \n");
            printf("\tl for login\n");
            printf("\ts for send message\n");
            printf("\tm for send message loop\n");
            printf("\tq for quit\n");
        }
        MyDebugPrint("cmd thread quit\n");
    });
    thr.detach();

    return app.Exec();
}

int main1(int argc, char *argv[])
{
#if 1
    int fd = -1, ret = 0;
    struct sockaddr_in addr;

    MyMsgInit();

    /// create
    fd = socket(AF_INET,SOCK_STREAM,0);
    assert(fd != -1);
    /// connect
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &addr.sin_addr);
    ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    assert(ret != -1);

    /// r/w
    int buf_len = 0;
    char* buf = NULL;

    /// login
    buf = MyMsgLogin(SRC,PASS,&buf_len);
    ret = write(fd,buf,buf_len);
    assert(ret == buf_len);

    getch();
    goto exit;
    while(getch() != 'q')
    {
        /// write msg
        char temp_buf[200] = {0};
        std::string msg = MSG;
        unsigned short msg_len = msg.size();
        memcpy(temp_buf,&msg_len,2);
        memcpy(&temp_buf[2],msg.c_str(),msg.size());
        buf = MyMsgBuildSingleMsg(SRC,DEST,temp_buf,msg.size()+2+1,&buf_len);
        ret = write(fd,buf,buf_len);
        assert(ret == buf_len);

        /// read msg
        char read_buf[1024] = {0};
        ret = read(fd,read_buf,1024);
        unsigned short code = MyMsgHandle(read_buf);
        SingleMsg_t msg1;
        switch (code) {
        case 0x0009:
            MyMsgHandleSingleMsg(read_buf,ret,&msg1);
            printf("src: %s\tdest: %s\n",msg1.src.c_str(),
                   msg1.dest.c_str());
            printf("msg is:");
            for(int i = 0; i < msg1.len; ++i)
            {
                printf("%02X\t",msg1.buf[i]);
            }
            printf("\n");
            break;
        default:
            printf("other msg\n");
            break;
        }
    }

exit:
    // exit
    close(fd);
    return MyMsgExit();
#else

#endif
}
