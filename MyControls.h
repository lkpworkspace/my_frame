#ifndef MYCONTROLS_H
#define MYCONTROLS_H
#include <map>
#include <string>

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

#endif // MYCONTROLS_H
