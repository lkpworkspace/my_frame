#ifndef MYPEOPLEITEM_H
#define MYPEOPLEITEM_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include "MyUdp.h"
#include <string>
#include "MyTalkWidget.h"

using namespace my_master;

class MyPeopleItem : public QWidget
{
    Q_OBJECT
public:
    explicit MyPeopleItem(MyAddrInfo info,
                          std::string name,
                          std::string account,
                          std::string mark,
                          QWidget *parent = 0);
    void SetName(std::string);
    void SetAccount(std::string account){m_account = account;}
    void SetAddr(MyAddrInfo);
    void SetMark(std::string);

    void AppendMsg(std::string msg){m_talk->AppendMsg(msg);m_talk->show();}
    std::string GetAccount(){return m_account;}

    MyAddrInfo m_info;
    std::string m_account;
    QWidget *head;         //头像
    QLabel *name;          //用户名
    QLabel *sign;          //个性签名
    QString headPath;      //头像路径
    bool eventFilter(QObject *obj, QEvent *event);//事件过滤器
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
signals:

public slots:
private:
    void initUi();//初始化Ui
    MyTalkWidget* m_talk;
};
#endif // MYPEOPLEITEM_H
