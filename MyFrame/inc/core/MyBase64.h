#ifndef __MYBASE64_H__
#define __MYBASE64_H__
#include <stdint.h>
namespace my_master {
class MyBase64
{
public:
    // 参数：
    //      (in)要编码的buf
    //      (in)该buf的长度
    //      (out)编完码后输出内存
    // 返回值：
    //      dst的大小
    static int base64_encode(uint8_t* src, int len, uint8_t** dst);
    static int base64_decode(uint8_t* src, int len, uint8_t** dst);
private:
    static void __base64_init();
};

}// end namespace


#endif
