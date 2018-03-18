#ifndef MYVEC_HPP
#define MYVEC_HPP
/*
 *  Demo:
 *  动态数组
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

    // TODO: 抽空把该数组的数据结构做成 ring buffer
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


#endif // MYVEC_HPP

