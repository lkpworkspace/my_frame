#include "MyMainWidget.h"
#include "ui_MyMainWidget.h"
MyMainWidget::MyMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyMainWidget)
{
    ui->setupUi(this);
    m_normal = new MyNormalEvent();

    connect(ui->pushButton_send,SIGNAL(clicked(bool)),this,SLOT(PushBegin()));
    connect(ui->pushButton_close,SIGNAL(clicked(bool)),this,SLOT(hide()));
}

MyMainWidget::~MyMainWidget()
{
    delete ui;
}



void MyMainWidget::PushBegin()
{
    m_normal->Work();
}
