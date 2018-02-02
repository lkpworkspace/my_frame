#ifndef MYOBJ_H
#define MYOBJ_H
#include "MyCommon.h"
#include <unordered_set>

class MyObj
{
public:
    enum OBJ_Identify{
        IDENTIFY_NONE = -1,
        IDENTIFY_TASK = 0,    // 0 ~ 99
        IDENTIFY_FD = 1000,
    };

    /// default name: null,default tag: -1.
    explicit MyObj();
    MyObj(std::string key);
    ~MyObj();

    ////////////////////////////////////////////
    /// 名字或者标签设置失败会返回错误，并不会覆盖原来的设置
    std::string& GetObjName();
    bool SetObjName(std::string name);

    int GetTag(){return m_tag;}
    int SetUniqueTag();
protected:

    void SetIdentify(int identify){ m_identify = identify; }
    int GetIdentify(){ return m_identify; }

private:

    std::string m_obj_name;
    int m_tag;
    int m_identify;
    static std::unordered_set<std::string> l_obj_name;
    static std::unordered_set<int> l_tag;
    static int l_count_name;
    static int l_count_tag;
};

#endif // MYOBJ_H


