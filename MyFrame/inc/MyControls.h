#ifndef MYCONTROLS_H
#define MYCONTROLS_H
#include <map>
#include <string>
#include <iostream>

#define REQUEST(name) MyCtrls::GetInst()->Get(name)
#define UNREG(name) MyCtrls::GetInst()->UnRegCtrl(name)
/*  maybe the UI control need inherit this class
 */
class MyCtrlObj
{
public:
    MyCtrlObj(std::string key = "");
    ~MyCtrlObj();
    std::string& GetObjName();
    // maybe not useful in gcc
    std::string& GetTypeName();

    virtual int Call(int num){ num = num; return 0; }
    virtual int Call(int num,void*){ num = num; return 0; }
private:
    std::string m_obj_name;
    std::string m_type_name;
    static int l_count;
};

class MyObj
{
public:
    MyObj(std::string key = "");
    ~MyObj();
    std::string& GetObjName();
    void SetObjName(std::string name);

private:
    std::string m_obj_name;
    static int l_counter;
};

/*  author: kpli
 *  date:   2017.5.21
 *  feature:
 *      connect between UI control and Model
 *  note:
 *      it's useful when our program need UI control,
 *      the model only access this class rather than access UI control directly.
 */
class MyCtrls
{
public:
    static MyCtrls* GetInst();
    MyCtrlObj* Get(const std::string);
    int RegCtrl(const std::string,MyCtrlObj *);
    int UnRegCtrl(const std::string);
private:
    MyCtrls();
    bool Find(const std::string);
    static MyCtrls* instance;

    std::map<std::string,MyCtrlObj*> m_ctrl;
};

template<typename T1,typename T2>
class MyMap
{
public:
    std::map<T1,T2>& GetMap();
    T2 Get(T1);
    bool Insert(T1,T2);
    bool Remove(T1);
    bool Find(T1);
private:
    std::map<T1,T2> m_members;
};

template<typename T1,typename T2>
std::map<T1,T2>& MyMap<T1,T2>::GetMap()
{
    return m_members;
}

template<typename T1,typename T2>
T2 MyMap<T1,T2>::Get(T1 t1)
{
#if 1
    if(m_members.find(t1) != m_members.end())
    {
        return m_members.find(t1)->second;
    }
    std::cout << "Get : do not have \"" << t1 << "\"" << std::endl;
    T2 temp;
    return temp;
#else
    return m_members.find(t1)->second;
#endif
}

template<typename T1,typename T2>
bool MyMap<T1,T2>::Insert(T1 t1,T2 t2)
{
    if(Find(t1))
    {
        std::cout << "insert : insert \"" << t1 << "\" fail, the same key named" << std::endl;
        return false;
    }
    m_members.insert(std::pair<T1,T2>(t1,t2));
    return true;
}

template<typename T1,typename T2>
bool MyMap<T1,T2>::Remove(T1 t1)
{
    if(!Find(t1))
    {
        std::cout << "remove : remove \"" << t1 << "\" fail, do not have this key" << std::endl;
        return true;
    }
    m_members.erase(t1);
    return true;
}

template<typename T1,typename T2>
bool MyMap<T1,T2>::Find(T1 t1)
{
    if(m_members.find(t1) != m_members.end())
        return true;
    return false;
}

#endif // MYCONTROLS_H
