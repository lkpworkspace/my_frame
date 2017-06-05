#include "MainWidget.h"
#include "ui_mainwidget.h"
#include <QPixmap>
#include <QByteArray>

MainWidget::MainWidget(std::__cxx11::string key, QWidget *parent) :
    QWidget(parent),
    MyCtrlObj(key),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::SetPic(const char* data)
{
    QPixmap pix;
    QByteArray ba;


}
