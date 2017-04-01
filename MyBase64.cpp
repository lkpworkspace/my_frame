#include "MyBase64.h"

using namespace my_master;

uint8_t g_de_tab[128];

uint8_t g_en_tab[] = {
    'A','B','C','D','E','F','G','H',
    'I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X',
    'Y','Z','a','b','c','d','e','f',
    'g','h','i','j','k','l','m','n',
    'o','p','q','r','s','t','u','v',
    'w','x','y','z','0','1','2','3',
    '4','5','6','7','8','9','+','/'
};

uint8_t g_base64_extra = '=';

void MyBase64::__base64_init()
{
    uint32_t i;

    for(i=0; i<sizeof(g_en_tab); ++i)
    {
        g_de_tab[g_en_tab[i]] = i;
    }
}

// 参数：
//      (in)要编码的buf
//      (in)该buf的长度
//      (out)编完码后输出内存
// 返回值：
//      dst的大小
int MyBase64::base64_encode(uint8_t* src, int len, uint8_t** dst)
{
    int loopCount = len / 3;                        // 循环次数
    int size = len % 3;                             // 余下的字节数
    int dst_buf_size = loopCount * 4 + 4;           // 分配目标内存大小
    uint8_t* d;
    uint8_t* s = src;

    d = *dst = (uint8_t*)malloc(dst_buf_size);

    for(int i = 0; i < loopCount; ++i)
    {
        d[0] = (s[0] & 0xfc) >> 2;
        d[1] = ((s[0] & 0x03) << 4) + ((s[1] & 0xf0) >> 4);
        d[2] = ((s[1] & 0x0f) << 2) + ((s[2] & 0xc0) >> 6);
        d[3] = s[2] & 0x3f;

        for(int j = 0; j < 4; ++j)
        {
            d[j] = g_en_tab[d[j]];
        }

        d += 4;
        s += 3;
    }

    if(size != 0)
    {
        if(size == 1)
        {
            d[0] = (s[0] & 0xfc) >> 2;
            d[1] = ((s[0] & 0x03) << 4);
            d[2] = 0x00;
            d[3] = 0x00;
        }else
        {
            d[0] = (s[0] & 0xfc) >> 2;
            d[1] = ((s[0] & 0x03) << 4) + ((s[1] & 0xf0) >> 4);
            d[2] = ((s[1] & 0x0f) << 4);
            d[3] = 0x00;
        }
        for(int j = 3 - size; j > 0; ++j)
        {
            d[j] = g_base64_extra;
        }
        d += 4;
    }
    *d = 0;
    return d - *dst;
}


int MyBase64::base64_decode(uint8_t* src, int len, uint8_t** dst)
{
    uint8_t* s = src;
    uint8_t* d;
    int loopCount = len / 4;
    int dst_buf_size = loopCount * 3 + 1;
    int size = 0;

    d = *dst = (uint8_t*)malloc(dst_buf_size);

    if(src[len-1] == g_base64_extra)
    {
        ++size;
        if(src[len-2] == g_base64_extra)
            ++size;
    }

    __base64_init();

    for(int i = 0; i < loopCount; ++i)
    {
        for(int j = 0; j < 4; ++j)
            s[j] = g_de_tab[s[j]];

        d[0] = (s[0] << 2) + ((s[1] & 0xf0) >> 4);
        d[1] = ((s[1] & 0xf) << 4) + ((s[2] & 0xfc)>> 2);
        d[2] = ((s[2] & 3) << 6) + s[3];

        d += 3;
        s += 4;
    }
    d -= size;
    *d = 0;
    return d - *dst;
}

