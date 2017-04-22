#include "MyMainWidget.h"
#include "ui_MyMainWidget.h"
MyMainWidget::MyMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyMainWidget)
{
    ui->setupUi(this);
    m_normal = new MyNormalEvent();

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(PushBegin()));
}

MyMainWidget::~MyMainWidget()
{
    delete ui;
}

void MyMainWidget::PushBegin()
{
    m_normal->Work();
}
