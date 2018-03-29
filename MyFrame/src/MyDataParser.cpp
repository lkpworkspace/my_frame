#include "MyDataParser.h"
#include <string.h>

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
