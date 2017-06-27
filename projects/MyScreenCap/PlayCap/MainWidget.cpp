#include "MainWidget.h"
#include "ui_mainwidget.h"
#include "MyRecvCap.h"
#include <QPixmap>
#include <QByteArray>

MainWidget::MainWidget(std::__cxx11::string key, QWidget *parent) :
    QWidget(parent),
    MyCtrlObj(key),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    MyRecvCap* recv = (MyRecvCap*)(CALL_CTRL("recvcap"));
    if(recv != nullptr)
        connect(recv,SIGNAL(GetPic(QByteArray)),this,SLOT(SetPic(QByteArray)));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::SetPic(QByteArray ba)
{
    // load pic from bytearray
    QPixmap temp;
    temp.loadFromData(ba,"png");

    // display to window
    ui->label->setPixmap(temp);
    ui->label->setScaledContents(true);
}
