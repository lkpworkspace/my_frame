#ifndef __MyList_test_h__
#define __MyList_test_h__
#include "MyFrame.h"
#include "MyTest.h"

class A : public MyNode
{
public:
    A(int a){
        this->a = a;
    }
    int a;
};
class B
{
public:
    B(int v){value = v;}
    static bool Cmp(const void* max, const void* min)
    {
        B* ma = (B*)max;
        B* mi = (B*)min;
        if(ma->value >= mi->value)
            return true;
        return false;
    }
public:
    int value;
};

class MyList_test
{
public:
    MyList_test()
    {

    }

    void test()
    {
        MyList list;
        list.AddTail(new A(10));

        A* begin = (A*)list.Begin();
        A* end = (A*)list.End();
        printf("begin pointer %p\n",begin);
        printf("end pointer %p\n",end);
        while(begin != end)
        {
            std::cout << ((A*)begin)->a << std::endl;
            printf("begin pointer %p\n",begin);
            begin = (A*)(begin->next);
            printf("begin pointer %p\n",begin);
        }
        std::getchar();
    }
};




#endif




#ifndef __MyList_test_h__
#define __MyList_test_h__
#include "MyFrame.h"

class MyList_test
{
    MyList_test()
    {

    }

    void test()
    {

    }
};




#endif
