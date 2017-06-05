#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include "MyControls.h"
#include <QWidget>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget, public MyCtrlObj
{
    Q_OBJECT

public:
    explicit MainWidget(std::string key,QWidget *parent = 0);
    ~MainWidget();

    void SetPic(const char* data);
private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
