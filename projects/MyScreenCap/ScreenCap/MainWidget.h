#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QScreen>
#include <QTimer>
#include "MyControls.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget, public MyCtrlObj
{
    Q_OBJECT

public:
    explicit MainWidget(std::string key,QWidget *parent = 0);
    ~MainWidget();

public slots:
    void BeginCap();
    void StartCap();
    void StopCap();
    void ButtonStartStop();

private:
    Ui::MainWidget *ui;
    QScreen* m_screen;
    QTimer* m_screencap_timer;
    bool m_is_start;
};

#endif // MAINWIDGET_H
