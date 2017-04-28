#ifndef MYMAINWIDGET_H
#define MYMAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include "MyFrame.h"
#include "MyNormalEvent.h"
#include "MyTalkWidget.h"
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

public slots:
    void PushBegin();

private:
    void InitWindow();
    void AddBg();
    void AddPeople();
    Ui::MyMainWidget *ui;
    QLabel *m_bg;
    MyPersonList* m_people_list;
    MyNormalEvent* m_normal;  // do nothing
};

#endif // MYMAINWIDGET_H
