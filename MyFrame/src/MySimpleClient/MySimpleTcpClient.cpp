#include "MySimpleClient/MySimpleTcpClient.h"

#ifdef linux
#include <unistd.h>
#include <fcntl.h>
#else
#endif

#define FRAME_BUFFER_SIZE 65535






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
    int flags = fcntl( mSock, F_GETFL, 0 );
    flags = b ? ( flags | O_NONBLOCK ) : ( flags & ~O_NONBLOCK);
    int result = fcntl( mSock, F_SETFL, flags );
#else
    u_long arg = b ? 1 : 0;
    int result = ioctlsocket( mSock, FIONBIO, &arg );
#endif
    return result;
}

int MySimpleTcpClient::SetReuseSock()
{
    return MyNet::SetReuseSock(mSock);
}

int32_t MySimpleTcpClient::Send(const char* inData, size_t inLen)
{
    int bytesSentCount = send( mSock, static_cast< const char* >( inData ), inLen, 0 );
    if( bytesSentCount < 0 )
    {
        printf( "[MyEasyTcpClient::Send]: Send %d bytes\n", bytesSentCount );
    }
    return bytesSentCount;
}

int32_t MySimpleTcpClient::Recv(void* inData, size_t inLen)
{
    int bytesReceivedCount = recv( mSock, static_cast< char* >( inData ), inLen, 0 );
    if( bytesReceivedCount < 0 )
    {
        printf( "[MyEasyTcpClient::Recv]: Receive %d bytes\n", bytesReceivedCount );
    }
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
    mParser.GetDataFromSocket(mParser.mRecvCache,len);
}

int MySimpleTcpClient::GetFrame(MyInputStream& outInputStream)
{
    char* buf = nullptr;
    uint32_t len = mParser.GetFrame(&buf);
    outInputStream.SetBuffer(buf,len*8);
    return len;
}

int MySimpleTcpClient::Socket()
{
    mSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
#ifdef linux
    if( mSock == -1 )
#else
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
