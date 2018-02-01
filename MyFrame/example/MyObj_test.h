#ifndef __MyObj_test_h__
#define __MyObj_test_h__
#include "MyFrame.h"
#include "MyTest.h"
#include "MyObj.h"

class MyObj_test
{
public:
    MyObj_test()
    {

    }

    void test()
    {
        MyObj obj("somthing");
        MyObj obj2("somthing");
        MyObj obj6("somthing");
        obj6.SetObjName("somthing");
        MyDebugPrint("%d\n",obj.SetUniqueTag());
        MyDebugPrint("%d\n",obj2.SetUniqueTag());
        MyDebugPrint("%d\n",obj6.SetUniqueTag());
        MyDebugPrint("%d\n",obj6.SetUniqueTag());
    }
};




#endif
