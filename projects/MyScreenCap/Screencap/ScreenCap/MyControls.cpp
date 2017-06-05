#include "MyControls.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <typeinfo>
int MyCtrlObj::l_count = 0;
MyCtrlObj::MyCtrlObj(std::string key)
{
    if(key == "")
    {
        char buf[20];
        memset(buf,0,sizeof(buf));
        sprintf(buf,"key%d",l_count);
        key = buf;
        l_count++;
    }
    m_obj_name = key;
    MyCtrls::GetInst()->RegCtrl(key,(void*)this);
    m_type_name = typeid(this).name();
}

MyCtrlObj::~MyCtrlObj()
{
    MyCtrls::GetInst()->UnRegCtrl(m_obj_name);
}

std::string& MyCtrlObj::GetObjName()
{
    return m_obj_name;
}

std::string& MyCtrlObj::GetTypeName()
{
    return m_type_name;
}

//////////////////////////////////////////
MyCtrls* MyCtrls::instance = NULL;

MyCtrls* MyCtrls::GetInst()
{
    if(!instance)
        instance = new MyCtrls();
    return instance;
}

MyCtrls::MyCtrls()
{

}

void* MyCtrls::Get(const std::string key)
{
    std::map<std::string,void*>::iterator iter = m_ctrl.find(key);
    if(iter != m_ctrl.end())
    {
        return iter->second;
    }
    std::cout << "Get : do not have \"" << key << "\"control" << std::endl;
    return NULL;
}

int MyCtrls::RegCtrl(const std::string key,void *obj)
{
    if(Find(key))
    {
        std::cout << "RegCtrl : reg \"" << key << "\"control fail, the same key named" << std::endl;
        return -1;
    }
    m_ctrl.insert(std::pair<std::string,void*>(key,obj));
    return 0;
}

int MyCtrls::UnRegCtrl(const std::string key)
{
    if(!Find(key))
    {
        std::cout << "UnRegCtrl : unreg \"" << key << "\"control fail, do not have this key" << std::endl;
        return -1;
    }
    m_ctrl.erase(key);
    return 0;
}

bool MyCtrls::Find(std::string key)
{
    std::map<std::string,void*>::iterator iter = m_ctrl.find(key);
    if(iter != m_ctrl.end())
        return true;
    return false;
}
