#ifndef __MYHEAP_H__
#define __MYHEAP_H__

/*
    最大堆与最小堆：
    问题：
        1. 如何建一个堆
        2. 如何选出堆顶值后， 重新调整为一个堆
    描述：
        如果是堆，移出堆顶元素后
        1. 将最后一个元素移到堆顶
        2. 将堆顶元素与左右孩子比较，选出最大值(或最小值)与之交换，
        如此反复
    例如：
        堆的调整(自上往下的调整)
        {13 38 27 49 76 65 49 97} // 堆
        {97 38 27 49 76 65 49}    // 13 和 97 交换
        {27 38 49 49 76 65 97}    // 调整后的堆
        {38 49 49 97 76 65 27}    // 27 和 97交换后在进行调整成的新堆
        无序序列的筛选：(自下向上的调整)
        如果序列看成完全二叉树，则最后一个非终端节点是n/2下取整(1...n)
        由此筛选从n/2个元素开始
        {49 38 65 97 76 13 27 49} 无序序列
        {49 38 65 49 76 13 27 97} 97被筛选之后的状态
        {49 38 13 49 76 65 27 97} 65被筛选之后的状态
        {49 38 13 49 76 65 27 97} 38被筛选之后的状态
        {13 38 27 49 76 65 49 97} 49被筛选之后的状态
    适用范围：
        存储类指针并且重载了大于小于等于操作符，或者基本值类型。
    Demo:

    MyHeap<int> heap(10);
    heap.Add(10);
    heap.Add(20);
    heap.Add(56);
    heap.Add(30);
    for(int i = 0; i < heap.Count(); ++i)
    {
        cout << heap.GetData(i) << endl;
    }
    cout << endl;
    while(!heap.IsEmpty())
        cout << heap.Pop() << endl;
*/
#if 0
template<typename T>
class MyHeap
{
public:
    // size: 定义数组的大小， cmp： 比较函数(可以使用蓝不达表达式), isBigHeap: 默认是大顶堆
    MyHeap(int size, bool (*cmp)(const void* max, const void* min) = NULL, bool isBigHeap = true);
    ~MyHeap(){__Destory();}

    void Add(T data);                          // 往堆中添加元素
    T Pop();                                   // 获得数组中的元素
    T GetData(int index){return ((T*)m_data)[index];}
    int Count(){return m_count;}               // 获得堆中的数量
    bool IsEmpty() {return m_count == 0;}      // 是否为空
private:
    void __Del();                    // 删除堆顶元素
    void __Destory();                // 清除函数
    void __AdjustAdd(int i);         // 添加函数
    void __AdjustDel();              // 删除函数
    void __HeapAdjust(int idx);      // 堆调整函数
    // true: a > b; false: a < b;
    bool __cmp(T a, T b){return a > b;} /* 默认的比较函数 */

    void** m_data;          /* T类型的数组指针 */
    int m_size;             /* 数组总大小 */
    int m_count;            /* 已使用的数量 */
    bool m_isBigHeap;       /* 是否使用大顶堆 */
    /* true: max > min; false: max < min */
    bool (*m_cmp)(const void* max, const void* min);
};

template<typename T>
MyHeap<T>::MyHeap(int size, bool (*cmp)(const void *max, const void *min), bool isBigHeap)
{
    m_size = size;
    m_count = 0;
    m_cmp = cmp;
    m_isBigHeap = isBigHeap;
    m_data = new void*[size];
}

template<typename T>
void MyHeap<T>::__Destory()
{
    delete[] m_data;
}

// parent: 传入要调整元素的下标
template<typename T>
void MyHeap<T>::__HeapAdjust(int parent)
{
    T temp = ((T*)m_data)[parent];
    int child = parent * 2 + 1;
    if(m_isBigHeap)
    {// 大顶堆
        while(child < m_count)
        {
            temp = ((T*)m_data)[parent];
            // 如果没有右节点，并且左节点小于父节点，不需要调整，返回
            //if(child + 1 >= m_count && m_data[child] <= m_data[parent])
            if(child + 1 >= m_count && m_cmp(((T*)m_data)[parent],((T*)m_data)[child]))
                break;
            // 如果有右节点，并且右孩子比左孩子大，则孩子节点换为右孩子
            //if(child + 1 < m_count && m_data[child] < m_data[child + 1])
            if(child + 1 < m_count && m_cmp(((T*)m_data)[child+1],((T*)m_data)[child]))
                ++child;
            // 如果父节点小于孩子节点，则将孩子赋值给父节点
            //if(m_data[parent] < m_data[child])
            if(m_cmp(((T*)m_data)[child],((T*)m_data)[parent]))
                ((T*)m_data)[parent] = ((T*)m_data)[child];
            parent = child;
            child = 2 * child + 1;
        }
    }else
    {// 小顶堆
        while(child < m_count)
        {
            temp = ((T*)m_data)[parent];
            // 如果没有右节点，并且左节点大于父节点，不需要调整，返回
            if(child + 1 >= m_count && ((T*)m_data)[child] > ((T*)m_data)[parent])
                break;
            // 如果有右节点，并且右孩子比左孩子小，则孩子节点换为右孩子
            if(child + 1 < m_count && ((T*)m_data)[child] > ((T*)m_data)[child + 1])
                ++child;
            // 如果父节点大于孩子节点，则将孩子赋值给父节点
            if(((T*)m_data)[parent] > ((T*)m_data)[child])
                ((T*)m_data)[parent] = ((T*)m_data)[child];
            parent = child;
            child = 2 * child + 1;
        }
    }
    ((T*)m_data)[parent] = temp;

}

template<typename T>
T MyHeap<T>::Pop()
{
    if(IsEmpty()) return NULL;
    T temp = ((T*)m_data)[0];
    __Del();
    return temp;
}

template<typename T>
void MyHeap<T>::Add(T data)
{
    /* 扩大内存 */
    if(m_count == m_size)
    {
        void** temp = new void*[m_size + 32];
        for(int i = 0; i < m_size; ++i)
        {
            ((T*)temp)[i] = ((T*)m_data)[i];
        }
        delete[] m_data;
        m_data = temp;
        m_size += 32;
    }
    // 加入堆
    ((T*)m_data)[m_count++] = data;
    __AdjustAdd(m_count - 1);
}

// 将元素加入最后，然后自低向上调整堆(i是插入数据的下标)
template<typename T>
void MyHeap<T>::__AdjustAdd(int i)
{/* i = {0...(n-1)/2} */
    int p;//父节点的下标

    while(i > 0)
    {
        p = (i - 1) / 2;
        __HeapAdjust(p);
        i = p;
    }
}

// 删除0下标的元素
template<typename T>
void MyHeap<T>::__Del()
{
    if(IsEmpty()) return;
    __AdjustDel();
}

// 删除0下标的元素
template<typename T>
void MyHeap<T>::__AdjustDel()
{
    ((T*)m_data)[0] = ((T*)m_data)[m_count - 1];
    --m_count;
    __HeapAdjust(0);
}
#else
template<typename T>
class MyHeap
{
public:
    // size: 定义数组的大小， cmp： 比较函数(可以使用蓝不达表达式), isBigHeap: 默认是大顶堆
    MyHeap(int size, bool (*cmp)(const void* max, const void* min) = NULL, bool isBigHeap = true);
    ~MyHeap(){__Destory();}

    void Add(T data);                          // 往堆中添加元素
    T Pop();                                   // 获得数组中的元素
    T GetData(int index){return m_data[index];}
    int Count(){return m_count;}               // 获得堆中的数量
    bool IsEmpty() {return m_count == 0;}      // 是否为空
private:
    void __Del();                    // 删除堆顶元素
    void __Destory();                // 清除函数
    void __AdjustAdd(int i);         // 添加函数
    void __AdjustDel();              // 删除函数
    void __HeapAdjust(int idx);      // 堆调整函数
    // true: a > b; false: a < b;
    bool __cmp(T a, T b){return a > b;} /* 默认的比较函数 */

    T* m_data;           /* T类型的数组指针 */
    int m_size;          /* 数组总大小 */
    int m_count;         /* 已使用的数量 */
    bool m_isBigHeap;    /* 是否使用大顶堆 */
    /* true: max > min; false: max < min */
    bool (*m_cmp)(const void* max, const void* min);
};

template<typename T>
MyHeap<T>::MyHeap(int size, bool (*cmp)(const void *max, const void *min), bool isBigHeap)
{
    m_size = size;
    m_count = 0;
    m_cmp = cmp;
    m_isBigHeap = isBigHeap;
    m_data = new T[size];
}

template<typename T>
void MyHeap<T>::__Destory()
{
    delete m_data;
}

// parent: 传入要调整元素的下标
template<typename T>
void MyHeap<T>::__HeapAdjust(int parent)
{
    T temp = m_data[parent];
    int child = parent * 2 + 1;
    if(m_isBigHeap)
    {// 大顶堆
        while(child < m_count)
        {
            temp = m_data[parent];
            // 如果没有右节点，并且左节点小于父节点，不需要调整，返回
            if(child + 1 >= m_count && m_data[child] <= m_data[parent])
                break;
            // 如果有右节点，并且右孩子比左孩子大，则孩子节点换为右孩子
            if(child + 1 < m_count && m_data[child] < m_data[child + 1])
                ++child;
            // 如果父节点小于孩子节点，则将孩子赋值给父节点
            if(m_data[parent] < m_data[child])
                m_data[parent] = m_data[child];
            parent = child;
            child = 2 * child + 1;
        }
    }else
    {// 小顶堆
        while(child < m_count)
        {
            temp = m_data[parent];
            // 如果没有右节点，并且左节点大于父节点，不需要调整，返回
            if(child + 1 >= m_count && m_data[child] > m_data[parent])
                break;
            // 如果有右节点，并且右孩子比左孩子小，则孩子节点换为右孩子
            if(child + 1 < m_count && m_data[child] > m_data[child + 1])
                ++child;
            // 如果父节点大于孩子节点，则将孩子赋值给父节点
            if(m_data[parent] > m_data[child])
                m_data[parent] = m_data[child];
            parent = child;
            child = 2 * child + 1;
        }
    }
    m_data[parent] = temp;

}

template<typename T>
T MyHeap<T>::Pop()
{
    if(IsEmpty()) return NULL;
    T temp = m_data[0];
    __Del();
    return temp;
}

template<typename T>
void MyHeap<T>::Add(T data)
{
    /* 扩大内存 */
    if(m_count == m_size)
    {
        T* temp = new T[m_size + 32];
        for(int i = 0; i < m_size; ++i)
        {
            temp[i] = m_data[i];
        }
        delete m_data;
        m_data = temp;
        m_size += 32;
    }
    // 加入堆
    m_data[m_count++] = data;
    __AdjustAdd(m_count - 1);
}

// 将元素加入最后，然后自低向上调整堆(i是插入数据的下标)
template<typename T>
void MyHeap<T>::__AdjustAdd(int i)
{/* i = {0...(n-1)/2} */
    int p;//父节点的下标

    while(i > 0)
    {
        p = (i - 1) / 2;
        __HeapAdjust(p);
        i = p;
    }
}

// 删除0下标的元素
template<typename T>
void MyHeap<T>::__Del()
{
    if(IsEmpty()) return;
    __AdjustDel();
}

// 删除0下标的元素
template<typename T>
void MyHeap<T>::__AdjustDel()
{
    m_data[0] = m_data[m_count - 1];
    --m_count;
    __HeapAdjust(0);
}
#endif


#endif
