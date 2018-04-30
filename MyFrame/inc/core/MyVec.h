#ifndef __MYVEC_H__
#define __MYVEC_H__

#include <iostream>

/**
 * 该数组使用 ring buffer 进行实现
 */
template<typename T>
class MyVec
{
public:
    MyVec(int step = 4)
        :m_size(step),
          m_step(step),
          m_vs(nullptr),
          m_head(0),
          m_tail(0)
    { Create(); }
    ~MyVec(){ Destory(); }

    /**
     * 添加到尾部
     */
    void PushBack(T data)
    {
        if((m_tail + 1) % m_size == m_head)
        {
            ReallocAndCopy();
        }
        m_vs[m_tail] = data;
        m_tail = (m_tail + 1) % m_size;
    }

    /**
     * 添加到头部
     */
    void PushFront(T data)
    {
        if((m_tail + 1) % m_size == m_head)
        {
            ReallocAndCopy();
        }
        if(m_head == 0)
        {
            m_vs[m_size] = data;
            m_head = m_size;
        }
        else
        {
            m_vs[m_head - 1] = data;
            m_head -= 1;
        }
    }

    /**
     * 删除
     */
    void Pop()
    {
        if(m_head == m_tail) return;
        m_head = (m_head + 1) % m_size;
    }

    T& Front()
    {
        return m_vs[m_head];
    }


    T& Back()
    {
        if(m_head != m_tail)
        {
            if(m_tail == 0)
                return m_vs[m_size];
            else
                return m_vs[m_tail - 1];
        }
    }

    bool IsEmpty()
    {
        return (m_head == m_tail);
    }

    /**
     * 返回当前数组中存储的元素数量
     */
    int Count()
    {
        return (m_tail - m_head + m_size) % m_size;
    }

    /**
     * 返回该下标的数据
     */
    T& Get(int index)
    {
        return m_vs[(m_head + index) % m_size];
    }

    /// MyVec 测试函数
    static void Test()
    {
        MyVec<int> vec;
        for(int i = 0; i < 16; ++i)
        {
            vec.PushBack(i);
        }

        vec.Pop();
        vec.Pop();
        vec.PushFront(10);
        vec.PushFront(10);

        for(int i = 0; i < 10; ++i)
        {
            if(!vec.IsEmpty())
            {
                std::cout << vec.Front() << std::endl;
                vec.Pop();
            }
        }

        vec.PushBack(0xff);
        std::cout << "count:" << vec.Count() << std::endl;
    }

private:
    void Create()
    {
        m_vs = (T*)new T[m_size];
    }

    void Destory()
    {
        if(m_vs != nullptr)
            delete[] m_vs;
    }

    void ReallocAndCopy()
    {
        T* temp = (T*)new T[m_size + m_step];
        int len = (m_tail - m_head + m_size) % m_size;
        for(int i = 0; i < len; ++i)
        {
            temp[i] = m_vs[(m_head + i)%m_size];
        }
        m_head = 0;
        m_tail = len;
        m_size += m_step;
        delete[] m_vs;
        m_vs = temp;
    }

private:
    T* m_vs;        /* malloc buffer */
    int m_head;     /* head index */
    int m_tail;     /* tail index */
    int m_size;     /* total size */
    int m_step;     /* when size not enough, make more */
};

#if 0
/**
 * 组该数组使用 动态数组 进行实现
 */
template<typename T>
class MyVec
{
public:
    MyVec(int step = 64)
        :m_count(0), m_size(step), m_step(step), m_vs(NULL)
    { Create(); }
    ~MyVec(){ Destory(); }

    /**
     * 添加
     */
    void Add(T &data);

    /**
     * 删除
     */
    void Remove(int index);

    /**
     * 快速删除(将最后一个移到要删除的位置)
     */
    void FastRemove(int index);

    /**
     * 返回当前数组中存储的元素数量
     */
    int Count(){return m_count;}

    /**
     * 返回该下标的数据
     */
    T &Get(int index);

    /**
     * 返回存储缓存指针
     */
    T* GetBuf(){ return m_vs; }


    static void Test()
    {
        MyVec<int> vec(10);
        vec.Add(10);
        vec.Add(20);
        vec.Add(30);

        for(int i = 0; i < vec.Count(); ++i)
        {
            cout << vec.Get(i) << endl;
        }
        vec.Remove(0);
        for(int i = 0; i < vec.Count(); ++i)
        {
            cout << vec.Get(i) << endl;
        }
    }

private:
    void Create();
    void Destory();
private:
    T* m_vs;        /* malloc buffer */
    int m_size;     /* total size */
    int m_count;    /* count now */
    int m_step;     /* when size not enough, make more */
};

template<typename T>
void MyVec<T>::Create()
{
    m_vs = (T*)new T[m_size];
}

template<typename T>
void MyVec<T>::Destory()
{
    if(m_vs == NULL)
        delete m_vs;
    m_vs = NULL;
}

template<typename T>
void MyVec<T>::Add(T& data)
{
    if(m_size == m_count)
    {
        int newsize = m_size + m_step;
        T* p = (T*)new T[newsize];
        int i;
        for(i = 0; i < m_size; ++i)
        {
            p[i] = m_vs[i];
        }
        delete m_vs;
        m_size = newsize;
        m_vs = p;
    }
    m_vs[m_count++] = data;
}

template<typename T>
void MyVec<T>::Remove(int index)
{
    if(index >= m_count || index < 0) return;
    for(;index < m_count-1; ++index)
    {
        m_vs[index] = m_vs[index+1];
    }
    m_count--;
}

template<typename T>
void MyVec<T>::FastRemove(int index)
{
    if(index >= m_count || index < 0) return;
    m_vs[index] = m_vs[m_count -1];
    m_count--;
}

template<typename T>
T& MyVec<T>::Get(int index)
{
    if(index >= m_count || index < 0) return NULL;
    return m_vs[index];
}
#endif

#endif // MYVEC_HPP

