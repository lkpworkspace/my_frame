#include <iostream>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <assert.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "MyMsg.h"


int main(int argc, char *argv[])
{
    int fd = -1, ret = 0;
    struct sockaddr_in addr;

    MyMsgInit();

    // create
    fd = socket(AF_INET,SOCK_STREAM,0);
    assert(fd != -1);
    // connect
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4399);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    assert(ret != -1);

    // r/w
    int buf_len = 0;
    char* buf = NULL;

    // login
    buf = MyMsgLogin("123456789","123456",&buf_len);
    ret = write(fd,buf,buf_len);
    assert(ret == buf_len);

    // send msg
    std::string msg = "hello, world";
    buf = MyMsgSingleMsg("123456789","123456789",msg.c_str(),msg.size(),&buf_len);
    ret = write(fd,buf,buf_len);
    assert(ret == buf_len);

    char read_buf[1024] = {0};
    ret = read(fd,read_buf,1024);
    for(int i = 0; i < ret; ++i)
    {
        printf("%02X\t", read_buf[i]);
    }

    // exit
    close(fd);
    return MyMsgExit();
}
