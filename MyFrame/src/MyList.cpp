#include "MyList.h"

// init
void MyList::__Init()
{
    m_root.prev = m_root.next = &m_root;
    m_count = 0;
}

// add
void MyList::__Add(MyNode* prev, MyNode* next, MyNode* node)
{
    if(!node) return;
    ++m_count;
    prev->next = node;
    next->prev = node;
    node->prev = prev;
    node->next = next;
}
void MyList::AddHead(MyNode* node)
{
    __Add(&m_root,m_root.next,node);
}
void MyList::AddTail(MyNode *node)
{
    __Add(m_root.prev,&m_root,node);
}

// del
void MyList::__Del(MyNode *prev, MyNode *next,bool b)
{
    --m_count;
    if(b)
        delete prev->next;
    prev->next = next;
    next->prev = prev;
}
void MyList::Del(MyNode *node,bool b)
{
    __Del(node->prev,node->next,b);
    node->prev = node->next = node;
}
void MyList::DelHead(bool b)
{
    if(IsEmpty()) return;
    //__Del(&m_root,m_root.next->next,b);
    Del(m_root.next,b);
}
void MyList::DelTail(bool b)
{
    if(IsEmpty()) return;
    //__Del(m_root.prev->prev,&m_root,b);
    Del(m_root.prev,b);
}
void MyList::DelWithIndex(int index,bool b)
{
    if(IsEmpty()) return;
    MyNode* temp;

    temp = GetData(index);
    if(temp)
        __Del(temp->prev,temp->next,b);
}
// move append函数
void MyList::MoveHead(MyNode* node)
{
    __Del(node->prev,node->next,false);
    AddHead(node);
}
void MyList::MoveTail(MyNode* node)
{
    __Del(node->prev,node->next,false);
    AddTail(node);
}
void MyList::Append(MyList *from)
{
    if(from->IsEmpty()) return;
    MyNode* f_head = from->m_root.next;
    MyNode* f_tail = from->m_root.prev;

    m_root.prev->next = f_head;
    f_head->prev = m_root.prev;

    f_tail->next = &m_root;
    m_root.prev = f_tail;

    m_count += from->m_count;
    // must invoke this method
    from->__Init();
}

MyNode* MyList::GetData(int index)
{
    if(IsEmpty()) return &m_root;
    if(index < 0 || index >= m_count) return nullptr;

    MyNode* temp = &m_root;
    int temp_index = -1;

    if(index >= m_count / 2)
    {// 使用倒序遍历
        temp_index = m_count;
        do{
            if(index != temp_index)
            {
                --temp_index;
                temp = temp->prev;
            }else
                return temp;
        }while(true);
    }else
    {// 使用正序遍历
        temp_index = -1;
        do{
            if(index != temp_index)
            {
                ++temp_index;
                temp = temp->next;
            }else
                return temp;
        }while(true);
    }
}

void MyList::Clear(bool b)
{
    while(!IsEmpty()){DelHead(b);}
    m_count = 0;
}

/*
template<typename T>
class MyNode_list
{
public:
    MyNode_list()
        :m_data(0),prev(NULL),next(NULL)
    {}
    MyNode_list(T data)
        :m_data(data),prev(NULL),next(NULL)
    {}
    MyNode_list<T>* prev;
    MyNode_list<T>* next;
    T GetData(){return m_data;}
    void SetData(T data){this->m_data = data;}
private:
    T m_data;
};
*/

//template<typename T>
//class MyList
//{
//public:
//    MyList(){__Init();}
//    ~MyList(){Clear(true);}
//    ////////////////////////////////////
//    void AddHead(T data);//
//    void AddTail(T data);//
//    void DelHead(bool b = true);//
//    void DelTail(bool b = true);//
//    void DelWithIndex(int index,bool b = true);
//    void Del(T data, bool b = true);
//    void Append(MyList<T>* from);
//    ////////////////////////////////////
//private:
//    void AddHead(MyNode_list<T>* node);//
//    void AddTail(MyNode_list<T>* node);//
//    void Del(MyNode_list<T>* node,bool b = true);//
//    void MoveHead(MyNode_list<T>* node);//
//    void MoveTail(MyNode_list<T>* node);//
//    ////////////////////////////////////
//public:
//    T GetData(int index);//
//    int Count(){return m_count;}//
//    bool IsEmpty(){return &m_root == m_root.next;}//
//    void Clear(bool b = true);//
//private:
//    void __Init();// 初始化
//    void __Add(MyNode_list<T>* prev, MyNode_list<T>* next, MyNode_list<T>* node);//
//    void __Del(MyNode_list<T>* prev, MyNode_list<T>* next, bool b);//
//    MyNode_list<T>* __GetDataWithIndex(int index);//
//    MyNode_list<T>* __GetData(T data);//

//    MyNode_list<T> m_root;// 链表头
//    int m_count;// 链表数量
//};

//// 初始化函数
//template<typename T>
//void MyList<T>::__Init()
//{
//    m_root.prev = m_root.next = &m_root;
//    m_count = 0;
//    m_root.SetData(NULL);
//}

//// add 函数
//template<typename T>
//void MyList<T>::__Add(MyNode_list<T>* prev, MyNode_list<T>* next, MyNode_list<T>* node)
//{
//    if(!node) return;
//    ++m_count;
//    prev->next = node;
//    node->prev = prev;
//    node->next = next;
//    next->prev = node;
//}

//template<typename T>
//void MyList<T>::AddHead(MyNode_list<T>* node)
//{
//    __Add(&m_root,m_root.next,node);
//}

//template<typename T>
//void MyList<T>::AddTail(MyNode_list<T> *node)
//{
//    __Add(m_root.prev,&m_root,node);
//}

//template<typename T>
//void MyList<T>::AddTail(T data)
//{
//    AddTail(new MyNode_list<T>(data));
//}

//template<typename T>
//void MyList<T>::AddHead(T data)
//{
//    AddHead(new MyNode_list<T>(data));
//}

//// del 函数
//template<typename T>
//void MyList<T>::__Del(MyNode_list<T> *prev, MyNode_list<T> *next,bool b)
//{
//    --m_count;
//    if(b)
//        delete prev->next;
//    prev->next = next;
//    next->prev = prev;
//}

//template<typename T>
//void MyList<T>::Del(MyNode_list<T> *node,bool b)
//{
//    __Del(node->prev,node->next,b);
//}

//template<typename T>
//void MyList<T>::DelHead(bool b)
//{
//    if(IsEmpty()) return;
//    __Del(&m_root,m_root.next->next,b);
//}

//template<typename T>
//void MyList<T>::DelTail(bool b)
//{
//    if(IsEmpty()) return;
//    __Del(m_root.prev->prev,&m_root,b);
//}

//template<typename T>
//void MyList<T>::DelWithIndex(int index,bool b)
//{
//    if(IsEmpty()) return;
//    MyNode_list<T>* temp;

//    temp = __GetDataWithIndex(index);
//    if(temp)
//        __Del(temp->prev,temp->next,b);
//}

//template<typename T>
//void MyList<T>::Del(T data, bool b)
//{
//    if(IsEmpty()) return;
//    MyNode_list<T>* temp;
//    temp = __GetData(data);
//    if(temp)
//        __Del(temp->prev,temp->next,b);
//}

//// move append函数
//template<typename T>
//void MyList<T>::MoveHead(MyNode_list<T>* node)
//{
//    __Del(node->prev,node->next,false);
//    AddHead(node);
//}

//template<typename T>
//void MyList<T>::MoveTail(MyNode_list<T>* node)
//{
//    __Del(node->prev,node->next,false);
//    AddTail(node);
//}

//template<typename T>
//void MyList<T>::Append(MyList<T>* from/*don't invoke*/)
//{
//    // TODO...
//    //    if(from->IsEmpty()) return;

//    //    MyList<T>* from_tail = from->__GetData(0);
//    //    MyList<T>* from_head = from->__GetData(from->Count() - 1);

//    //    to->__GetData(to->Count() - 1)->next = from_head;
//    //    from_head->prev = to->prev;

//    //    from_tail->next = to;
//    //    to->prev = from_tail;

//    //    my_list_init(from);
//    m_root.prev->next = from->m_root.next;
//    from->m_root.next->prev = m_root.prev;
//    m_root.prev = from->m_root.prev;
//    from->m_root.prev->next = &m_root;
//    m_count += from->m_count;
//}

//// get 函数
//template<typename T>
//MyNode_list<T>* MyList<T>::__GetDataWithIndex(int index)
//{
//    if(IsEmpty()) return NULL;
//    if(index < 0 || index >= m_count) return NULL;

//    MyNode_list<T>* temp = &m_root;
//    int temp_index = -1;

//    if(index >= m_count / 2)
//    {// 使用倒序遍历
//        temp_index = m_count;
//        do{
//            if(index != temp_index)
//            {
//                --temp_index;
//                temp = temp->prev;
//            }else
//                return temp;
//        }while(true);
//    }else
//    {// 使用正序遍历
//        temp_index = -1;
//        do{
//            if(index != temp_index)
//            {
//                ++temp_index;
//                temp = temp->next;
//            }else
//                return temp;
//        }while(true);
//    }
//}

//template<typename T>
//MyNode_list<T>* MyList<T>::__GetData(T data)
//{
//    if(IsEmpty()) return NULL;
//    MyNode_list<T>* temp = &m_root;
//    do{
//        temp = temp->next;
//        if(temp->GetData() == data)
//            return temp;
//    }while(temp != &m_root);
//    return NULL;
//}

//template<typename T>
//T MyList<T>::GetData(int index)
//{
//    MyNode_list<T>* temp;
//    if((temp = __GetDataWithIndex(index)) == NULL) return NULL;
//    return temp->GetData();
//}

//// clear 函数
//template<typename T>
//void MyList<T>::Clear(bool b)
//{
//    while(!IsEmpty()){DelHead(b);}
//    m_count = 0;
//}
