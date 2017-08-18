#ifndef MYMAINWIDGET_H
#define MYMAINWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QTimer>
#include "MyDelegate.h"
#include "MyFrame.h"
#include "MyNormalEvent.h"
#include "MyPeopleList.h"
using namespace my_master;
namespace Ui {
class MyMainWidget;
}

class MyMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyMainWidget(QWidget *parent = 0);
    ~MyMainWidget();
    void AddFriend(MyAddrInfo info,
                   std::string name,
                   std::string account,
                   std::string mark)
    {
        m_people_list->slotAddFriend(info,name,account,mark);
    }


protected:
    void closeEvent(QCloseEvent *event){qApp->exit();}
public slots:
    //void PushBegin(); // do nothing
    void AppendMsg(QString, QString);
    void AddMyFriend(QString,
                     unsigned short,
                     QString,
                     QString,
                     QString);
private:
    void InitWindow();
    void AddBg();
    MyPersonList* m_people_list;
    Ui::MyMainWidget *ui;
    QLabel *m_bg;
    //MyNormalEvent* m_normal;  // do nothing (only test)
};

#endif // MYMAINWIDGET_H
