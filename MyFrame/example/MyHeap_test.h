#ifndef __MyHeap_test_h__
#define __MyHeap_test_h__
#include "MyFrame.h"
#include "MyTest.h"
#include "MyHeap.h"

using namespace my_master;

class MyHeap_test
{
public:
    MyHeap_test(){

    }

    void test()
    {
#if 0
    MyHeap<B*> heap(1024,&B::Cmp,true);
    B* b1 = new B(2);
    B* b2 = new B(5);
    B* b3 = new B(4);
    B* b4 = new B(9);
    B* b5 = new B(3);
    heap.Add(b1);
    heap.Add(b2);
    heap.Add(b3);
//    heap.Add(b4);
//    heap.Add(b5);
    for(int i = 0; i < heap.Count();++i)
    {
        printf("%d\n",heap.GetData(i)->value);
    }
#endif
    MyHeap<int> heap(10);
    heap.Add(10);
    heap.Add(20);
    heap.Add(56);
    heap.Add(30);
    for(int i = 0; i < heap.Count(); ++i)
    {
        std::cout << heap.GetData(i) << std::endl;
    }
    std::cout << std::endl;
    while(!heap.IsEmpty())
        std::cout << heap.Pop() << std::endl;
    }
};

#endif
