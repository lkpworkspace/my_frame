#ifndef __MYLIST_H__
#define __MYLIST_H__
/*
 *  demo:
 *      双向循环链表
 *   #include <iostream>
 *   #include "MyList.h"
 *
 *   using namespace std;
 *   using namespace my_master
 *
 *   int main()
 *   {
 *       MyList list;
 *
 *       return 0;
 *   }
 *   problem:
 *         use MyNode_list or use data
 *         maybe use MyNode_list is ok
 *         eg: MyList<MyNode_list<int>*> list;
 *         no no no...
 *
 *         class A : public MyNode_list{};
 *         MyList<MyNode_list*> list;
 *         ok ok gogogo
 */

class MyNode
{
public:
    MyNode()
        :prev(nullptr),next(nullptr)
    {}
    virtual ~MyNode(){}
    MyNode* prev;
    MyNode* next;
};

class MyList
{
public:
    MyList(){__Init();}
    ~MyList(){Clear(false);}
    ////////////////////////////////////
    void AddHead(MyNode* node);
    void AddTail(MyNode* node);
    void Del(MyNode* node, bool b = false);
    void DelHead(bool b = false);
    void DelTail(bool b = false);
    void DelWithIndex(int index,bool b = false); // not useful
    void MoveHead(MyNode* node);
    void MoveTail(MyNode* node);
    void Append(MyList* from);
    MyNode* Begin(){return m_root.next;}
    MyNode* End(){return &m_root;}
public:
    MyNode* GetData(int index);    // not useful
    int Count(){return m_count;}
    bool IsEmpty(){return &m_root == m_root.next;}
    void Clear(bool b = false);
private:
    void __Init();
    void __Add(MyNode* prev, MyNode* next, MyNode* node);
    void __Del(MyNode* prev, MyNode* next, bool b);

    MyNode m_root;
    int m_count;
};

#endif // MY_LIST_H


