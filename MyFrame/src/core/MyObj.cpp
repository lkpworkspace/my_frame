#include "MyObj.h"
#include <string.h>

std::unordered_set<std::string> MyObj::l_obj_name;
std::unordered_set<int> MyObj::l_tag;
int MyObj::l_count_name = 0;
int MyObj::l_count_tag = 0;

MyObj::MyObj()
    :m_identify(-1),
      m_tag(-1),
      m_obj_name("null")
{}

MyObj::MyObj(std::string key)
    :m_identify(-1),
      m_tag(-1)
{
#if 0
    auto name = l_obj_name.find(key);
    if(name == l_obj_name.end())
    {
        l_obj_name.insert(key);
        l_count_name++;
    }else
    {
        char buf[64];
        memset(buf,0,sizeof(buf));
        sprintf(buf,"__key_%d",l_count_name++);
        MyDebugPrint("[Warning]: Object has key [%s], renamed [%s]\n",key.c_str(),buf);
        key = buf;
        l_obj_name.insert(key);
    }
#endif
    m_obj_name = key;
}

MyObj::~MyObj()
{
#if 0
    auto k = l_obj_name.find(m_obj_name);
    if(k != l_obj_name.end())
        l_obj_name.erase(m_obj_name);
#endif
    auto t = l_tag.find(m_tag);
    if(t != l_tag.end())
        l_tag.erase(m_tag);
}

std::string& MyObj::GetObjName()
{
    return m_obj_name;
}

bool MyObj::SetObjName(std::string name)
{
#if 0
    auto k = l_obj_name.find(name);
    if(k == l_obj_name.end())
    {
        l_obj_name.erase(m_obj_name);
        l_obj_name.insert(name);
        m_obj_name = name;
    }else
    {
        MyDebugPrint("[Warning]: Object has key [%s]\n",name.c_str());
        return false;
    }
#else
    m_obj_name = name;
#endif
    return true;
}

int MyObj::SetObjUniqueTag()
{
    if(m_tag != -1) return m_tag;
    m_tag = l_count_tag++;
    if(l_tag.find(m_tag) == l_tag.end())
        l_tag.insert(m_tag);
    else
    {
        printf("[MyObj::SetObjUniqueTag Warning]: Object has tag [%d]\n",m_tag);
        m_tag = -1;
    }

    return m_tag;
#if 0
    auto t = l_tag.find(tag);
    if(t == l_tag.end())
    {
        l_tag.erase(m_tag);
        l_tag.insert(tag);
        m_tag = tag;
        l_count_tag++;
    }else
    {
        MyDebugPrint("Object has tag [%d]\n",tag);
    }
#endif
}
