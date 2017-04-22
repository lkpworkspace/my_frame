#ifndef MYMAINWIDGET_H
#define MYMAINWIDGET_H

#include <QWidget>
#include "MyFrame.h"
#include "MyNormalEvent.h"
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
    Ui::MyMainWidget *ui;
    MyNormalEvent* m_normal;
};

#endif // MYMAINWIDGET_H
