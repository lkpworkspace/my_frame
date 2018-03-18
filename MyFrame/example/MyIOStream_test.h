#ifndef MYIOSTREAM_TEST_H
#define MYIOSTREAM_TEST_H

#include "MyFrame.h"
#include "MyTest.h"

/**
 * 示例程序简述：
 */

class MyIOStream_test
{
public:
    /**
     * 入口函数
     */
    static void Test()
    {
        MyOutputStream mbs;

        mbs.WriteBits( 11, 5 );
        mbs.WriteBits( 52, 6 );
#if 0
        MyMsgPool msgpool;
        msgpool.RegMsg("MSG_INPUT_STREAM", MyInputStream::StaticCreate);
        MyInputStream *in = (MyInputStream *)msgpool.Get("MSG_INPUT_STREAM");

        in->SetObjName("MSG_INPUT_STREAM");
        in->SetBuffer(mbs.GetBufferPtr(), mbs.GetByteLength() * 8);

        char ch;
        in->ReadBits(&ch,5);
        std::cout << (int)ch << std::endl;
        in->ReadBits(&ch,6);
        std::cout << (int)ch << std::endl;
#endif
    }
};

#endif
