#include "MyMsg.h"
#include <string.h>
#include <stdlib.h>
#include <string>
char *GetBuf(int *len);
void FreeBuf(char* buf);
uint16_t HandleHeader(const char *buf);
uint16_t HandleShort(int offset, const char* buf, int len);
uint8_t HandleChar(int offset, const char *buf, int len);
std::string HandleString(int offset, const char *buf, int len);
data_t HandleData(int offset, const char* buf, int len);
int BuildHeader(uint16_t head, char* buf, int len);
int BuildShort(uint16_t datalen, int offset, char* buf, int len);
int BuildChar(uint8_t ch,int offset, char* buf, int len);
int BuildString(const char* str, int offset, char* buf, int len);
int BuildData(const char* data, uint16_t data_len, int offset, char* buf, int len);

static char* g_buf = NULL;
static int g_buf_len = 0;

int MyMsgInit()
{
    g_buf = GetBuf(&g_buf_len);
    return 0;
}

int MyMsgExit()
{
    FreeBuf(g_buf);
    g_buf = NULL;
    g_buf_len = 0;
    return 0;
}

char* MyMsgLogin(std::string account,
                 std::string pass,
                 int* outlen)
{
    int index = 0;
#ifdef USE_HEAD
    index += 2;
#endif
    index += BuildShort(0x0001,index,g_buf,g_buf_len);
    index += BuildString(account.c_str(),index,g_buf,g_buf_len);
    index += BuildString(pass.c_str(),index,g_buf,g_buf_len);
#ifdef USE_HEAD
    BuildHeader((uint16_t)(index - 2),g_buf,g_buf_len);
#endif
    *outlen = index;
    return g_buf;
}

char* MyMsgSingleMsg(std::string src,
                     std::string dest,
                     const char* buf,
                     unsigned short len,
                     int *outlen)
{
    int index = 0;
#ifdef USE_HEAD
    index += 2;
#endif
    index += BuildShort(0x0009,index,g_buf,g_buf_len);
    index += BuildString(src.c_str(),index,g_buf,g_buf_len);
    index += BuildString(dest.c_str(),index,g_buf,g_buf_len);
    index += BuildData(buf,len,index,g_buf,g_buf_len);
#ifdef USE_HEAD
    BuildHeader((uint16_t)(index - 2),g_buf,g_buf_len);
#endif
    *outlen = index;
    return g_buf;
}
























////////////////////////////////////////////////////////////
/// MySelfProtocol
char *GetBuf(int *len)
{
    if(len != NULL)
        *len = MYPROTO_MAX_BUF_SIZE;
    return (char*)malloc(MYPROTO_MAX_BUF_SIZE);
}

void FreeBuf(char* buf)
{
    if(buf != NULL)
        free(buf);
    buf = NULL;
}

////////////////////////////////////////////////
/// handle
uint16_t HandleHeader(const char *buf)
{
    uint16_t head;
    memcpy(&head,&buf[0],sizeof(uint16_t));
    return head;
}

uint16_t HandleShort(int offset, const char* buf, int len)
{
    if(offset + sizeof(uint16_t) > len)
        return 0;
    uint16_t data_len = 0;
    memcpy(&data_len,&buf[offset],sizeof(data_len));
    return data_len;
}

uint8_t HandleChar(int offset, const char *buf, int len)
{
    if(offset + sizeof(uint8_t) > len)
        return 0;
    uint8_t data_len = 0;
    memcpy(&data_len,&buf[offset],sizeof(uint8_t));
    return data_len;
}

std::string HandleString(int offset, const char *buf, int len)
{
    char temp_buf[MYPROTO_MAX_BUF_SIZE] = {0};
    int str_len = strlen(&buf[offset]);
    memcpy(temp_buf,&buf[offset],str_len);
    return std::string(temp_buf);
}

data_t HandleData(int offset, const char* buf, int len)
{
    data_t my_data;

    my_data.len = HandleShort(offset,buf,len);
    memcpy(my_data.buf,&buf[offset + sizeof(uint16_t)],my_data.len);
    return my_data;
}

////////////////////////////////////////////////
/// build
int BuildHeader(uint16_t head, char* buf, int len)
{
    memcpy(buf,&head,sizeof(head));
    return sizeof(head);
}

int BuildShort(uint16_t datalen, int offset, char* buf, int len)
{
    if(offset + sizeof(datalen) > len)
        return 0;
    memcpy(&buf[offset],&datalen,sizeof(datalen));
    return sizeof(datalen);
}

int BuildChar(uint8_t ch,int offset, char* buf, int len)
{
    if(offset + sizeof(ch) > len)
        return 0;
    memcpy(&buf[offset],&ch,sizeof(ch));
    return sizeof(ch);
}

int BuildString(const char* str, int offset, char* buf, int len)
{
    int str_len = strlen(str);
    if(str_len + 1 + offset > len)
        return 0;
    memcpy(&buf[offset],str,str_len);
    return (str_len + 1);
}

int BuildData(const char* data, uint16_t data_len, int offset, char* buf, int len)
{
    int len_offset = BuildShort(data_len,offset,buf,len);
    if(data_len + offset + len_offset > len)
        return 0;
    memcpy(&buf[offset + len_offset],data,data_len);
    return (data_len + len_offset);
}
