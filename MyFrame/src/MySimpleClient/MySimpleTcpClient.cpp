#include "MySimpleClient/MySimpleTcpClient.h"
#include <stdio.h>
#ifdef linux
#include <unistd.h>
#include <fcntl.h>
#else
#endif

#define FRAME_BUFFER_SIZE 65535


MySimpleTcpClient::MySimpleTcpClient()
{
    Init();
    Socket();
}

MySimpleTcpClient::MySimpleTcpClient(MyAddrInfo &inAddrInfo)
    :mAddrInfo(inAddrInfo)
{
    Init();
    Socket();
}

MySimpleTcpClient::~MySimpleTcpClient()
{
    Exit();
}

int MySimpleTcpClient::Connect()
{
    int ret;
    /*
          此处先连接后,再设置未不阻塞,否则connect会立即返回
      */
    ret = connect(mSock, (struct sockaddr*)&mAddrInfo.GetAddr(), sizeof(mAddrInfo.GetAddr()));
    if(ret < 0)
    {
        printf("connect fail\n");
    }
    SetNonBlocking(true);
    return ret;
}

int MySimpleTcpClient::SetNonBlocking(bool b)
{
#ifdef linux
    int flags = fcntl(mSock, F_GETFL, 0);
    if(b)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;
    return fcntl(mSock, F_SETFL, flags);
#else
    u_long arg = b ? 1 : 0;
    return ioctlsocket( mSock, FIONBIO, &arg );
#endif
}

int MySimpleTcpClient::SetReuseSock()
{
    return MyNet::SetReuseSock(mSock);
}

void MySimpleTcpClient::SetAddrInfo(MyAddrInfo& info)
{
    mAddrInfo = info;
}

int32_t MySimpleTcpClient::Send(const char* inData, size_t inLen)
{
#ifdef linux
    int bytesSentCount = write( mSock, (const void*)inData, inLen);
#else
    int bytesSentCount = send( mSock, static_cast< const char* >( inData ), inLen, 0 );
#endif
    if( bytesSentCount < 0 )
    {
        printf( "[MyEasyTcpClient::Send]: Send %d bytes\n", bytesSentCount );
    }
    return bytesSentCount;
}

int32_t MySimpleTcpClient::Recv(void* inData, size_t inLen)
{
#ifdef linux
    int bytesReceivedCount = read( mSock, (char*)inData, inLen);
#else
    int bytesReceivedCount = recv( mSock, (char*)inData, inLen, 0 );
#endif
    if( bytesReceivedCount < 0 )
    {
        perror("recv");
        printf( "[MyEasyTcpClient::Recv]: Receive %d bytes, error %d\n", bytesReceivedCount ,errno);
    }
    printf( "[MyEasyTcpClient::Recv]: Receive %d bytes, error %d\n", bytesReceivedCount ,errno);
    return bytesReceivedCount;
}

int32_t MySimpleTcpClient::EasySend(const char* inData, size_t inLen)
{
    char* temp_buf = nullptr;
    int temp_len = mParser.WriteFrame(inData,inLen,&temp_buf);
    return Send(temp_buf,temp_len);
}

void MySimpleTcpClient::EasyRecv()
{
    int len = Recv(mParser.mRecvCache,FRAME_BUFFER_SIZE);
    if(len > 0)
        mParser.GetDataFromSocket(mParser.mRecvCache,len);
    else if(len == 0)
    {
        printf("disconnect from server\n");
    }
}

int MySimpleTcpClient::GetFrame(MyInputStream& outInputStream)
{
    char* buf = nullptr;
    uint32_t len = mParser.GetFrame(&buf);
    if(len > 0)
        outInputStream.SetBuffer(buf,len*8);
    return len;
}

int MySimpleTcpClient::Socket()
{
#ifdef linux
    mSock = socket( AF_INET, SOCK_STREAM, 0 );
    if( mSock == -1 )
#else
    mSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if( mSock == INVALID_SOCKET )
#endif
    {
        printf( "[MyEasyTcpClient::Socket]: Create Socket Failed\n" );
    }
    return mSock;
}

bool MySimpleTcpClient::Init()
{
#ifdef linux
    return true;
#else
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if ( iResult != NO_ERROR )
    {
        printf("[MyEasyTcpClient::Init]: Starting Up\n");
        return false;
    }

    return true;
#endif
}

void MySimpleTcpClient::Exit()
{
#ifdef linux
    close( mSock );
#else
    closesocket( mSock );
    WSACleanup();
#endif
}
