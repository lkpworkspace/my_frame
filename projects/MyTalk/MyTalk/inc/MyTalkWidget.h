#ifndef MYTALKWIDGET_H
#define MYTALKWIDGET_H
#include <QWidget>
#include <string>
#include <QLabel>
#include "MyUdp.h"
using namespace my_master;
namespace Ui {
class MyTalkWidget;
}

class MyTalkWidget : public QWidget
{
    Q_OBJECT

public:
    MyTalkWidget(MyAddrInfo info,
                          std::string name,
                          std::string account,
                          std::string mark,
                          QWidget *parent = 0);
    ~MyTalkWidget();

    void SetAddr(MyAddrInfo info){m_info = info;}
    void SetName(std::string name){m_name = name; Update();}
    void SetMark(std::string mark){m_mark = mark;}

    void AppendMsg(std::string msg);

public slots:
    void SendMsg();
private:
    void InitWindow();
    void Update();
    Ui::MyTalkWidget *ui;

    MyAddrInfo m_info;
    std::string m_account;
    std::string m_name;
    std::string m_mark;
};

#endif // MYTALKWIDGET_H
