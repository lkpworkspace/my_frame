#include "MySimpleClient/MyEasyTcpClient.h"

#ifdef linux
#include <unistd.h>
#include <fcntl.h>
#else
#endif

#define FRAME_BUFFER_SIZE 65535

MyDataParser::MyDataParser()
    :mFrameBuffer(nullptr),
      mWriteBuffer(nullptr),
      mRecvCache(nullptr)
{
    CreateFrameBuffer();
}

MyDataParser::~MyDataParser()
{
    DestoryFrameBuffer();
}

void MyDataParser::GetDataFromSocket(const char* inBuf, int inLen)
{
    for(int i = 0; i < inLen; ++i)
    {
        mRecvDataCache.PushBack(inBuf[i]);
    }
}

int MyDataParser::GetFrame(char** outBuf)
{
    char temp_buf[2] = {0};
    unsigned short frame_len = 0;
    if(mRecvDataCache.Count() >= 2)
    {
        temp_buf[0] = mRecvDataCache.Front();
        mRecvDataCache.Pop();
        temp_buf[1] = mRecvDataCache.Front();
        mRecvDataCache.Pop();
        memcpy(&frame_len, temp_buf,sizeof(frame_len));
        if(mRecvDataCache.Count() >= frame_len)
        {
            for(unsigned short i = 0; i < frame_len; ++i)
            {
                mFrameBuffer[i] = (char)mRecvDataCache.Front();
                mRecvDataCache.Pop();
            }
            *outBuf = mFrameBuffer;
            return frame_len;
        }else
        {
            mRecvDataCache.PushFront((char)temp_buf[1]);
            mRecvDataCache.PushFront((char)temp_buf[0]);
        }
    }
    return 0;
}

int MyDataParser::WriteFrame(const char *inBuf, int inLen, char **outBuf)
{
    memcpy(mWriteBuffer,&inLen,2);
    memcpy(&mWriteBuffer[2],inBuf,inLen);
    *outBuf = mWriteBuffer;
    return (inLen + 2);
}


void MyDataParser::CreateFrameBuffer()
{
    if(mFrameBuffer == nullptr)
        mFrameBuffer = (char*)malloc(FRAME_BUFFER_SIZE);
    if(mWriteBuffer == nullptr)
        mWriteBuffer = (char*)malloc(FRAME_BUFFER_SIZE);
    if(mRecvCache == nullptr)
        mRecvCache = (char*)malloc(FRAME_BUFFER_SIZE);
}

void MyDataParser::DestoryFrameBuffer()
{
    if(mFrameBuffer != nullptr)
        free(mFrameBuffer);
    if(mWriteBuffer != nullptr)
        free(mWriteBuffer);
    if(mWriteBuffer != nullptr)
        free(mRecvCache);
}

void MyDataParser::Test()
{
    MyDataParser parser;
    char temp_buf[] = {0x06,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x01,0x00,0x55};
    char* frame = nullptr;
    char temp_buf2[] = {0x06,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x01,0x00,0x55};
    char* frame2 = nullptr;
    int len = 0;

    parser.GetDataFromSocket(temp_buf,sizeof(temp_buf));
    len = parser.GetFrame(&frame);
    len = parser.GetFrame(&frame);
    len = parser.GetFrame(&frame);

    len = parser.WriteFrame(temp_buf2,sizeof(temp_buf2),&frame2);
    std::cout << len << std::endl;
}




MyEasyTcpClient::MyEasyTcpClient(MyAddrInfo &inAddrInfo)
    :mAddrInfo(inAddrInfo)
{
    Init();
    Socket();
}

MyEasyTcpClient::~MyEasyTcpClient()
{
    Exit();
}

int MyEasyTcpClient::Connect()
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

int MyEasyTcpClient::SetNonBlocking(bool b)
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

int MyEasyTcpClient::SetReuseSock()
{
    return MyNet::SetReuseSock(mSock);
}

int32_t MyEasyTcpClient::Send(const char* inData, size_t inLen)
{
    int bytesSentCount = send( mSock, static_cast< const char* >( inData ), inLen, 0 );
    if( bytesSentCount < 0 )
    {
        printf( "[MyEasyTcpClient::Send]: Send %d bytes\n", bytesSentCount );
    }
    return bytesSentCount;
}

int32_t MyEasyTcpClient::Recv(void* inData, size_t inLen)
{
    int bytesReceivedCount = recv( mSock, static_cast< char* >( inData ), inLen, 0 );
    if( bytesReceivedCount < 0 )
    {
        printf( "[MyEasyTcpClient::Recv]: Receive %d bytes\n", bytesReceivedCount );
    }
    return bytesReceivedCount;
}

int32_t MyEasyTcpClient::EasySend(const char* inData, size_t inLen)
{
    char* temp_buf = nullptr;
    int temp_len = mParser.WriteFrame(inData,inLen,&temp_buf);
    return Send(temp_buf,temp_len);
}

void MyEasyTcpClient::EasyRecv()
{
    int len = Recv(mParser.mRecvCache,FRAME_BUFFER_SIZE);
    mParser.GetDataFromSocket(mParser.mRecvCache,len);
}

int MyEasyTcpClient::GetFrame(char **outBuf)
{
    return mParser.GetFrame(outBuf);
}

int MyEasyTcpClient::Socket()
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

bool MyEasyTcpClient::Init()
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

void MyEasyTcpClient::Exit()
{
#ifdef linux
    close( mSock );
#else
    closesocket( mSock );
    WSACleanup();
#endif
}
