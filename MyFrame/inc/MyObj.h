#ifndef MYOBJ_H
#define MYOBJ_H
#include <unordered_set>

class MyObj
{
public:
    /**
     * 子类唯一标识的取值范围。
     * MyTask取值范围: 0~99
     * ...
     */
    enum OBJ_IDENTIFY{
        IDENTIFY_NONE = -1,
        IDENTIFY_TASK = 0,
        IDENTIFY_FD = 1000,
    };

    /**
     * default name: null,default tag: -1.
     */
    explicit MyObj();
    MyObj(std::string key);
    ~MyObj();

    /**
     * 名字或者标签设置失败会返回错误，并不会覆盖原来的设置
     */
    std::string& GetObjName();
    bool SetObjName(std::string name);
    int GetObjTag(){return m_tag;}
    int SetObjUniqueTag();

    /**
     * 用于子类之间获得唯一标识，唯一标识由子类维护，
     * 此处只给出具体子类标识的取值范围, 见 enum OBJ_IDENTIFY
     */
    int GetIdentify(){ return m_identify; }

protected:
    /**
     * 用于子类之间设置唯一标识，唯一标识由子类维护，
     * 此处只给出具体子类标识的取值范围, 见 enum OBJ_IDENTIFY
     */
    void SetIdentify(int identify){ m_identify = identify; }

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


