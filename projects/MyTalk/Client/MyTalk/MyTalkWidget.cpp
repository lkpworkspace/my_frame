#include "MyTalkWidget.h"
#include "ui_MyTalkWidget.h"
#include <QDebug>
#include "MyTalkClient.h"
using namespace my_master;

MyTalkWidget::MyTalkWidget(MyAddrInfo info, std::string name, std::string mark, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyTalkWidget)
{
    QLabel* m_bg = new QLabel(this);
    m_bg->setPixmap(QPixmap("../bg.jpg"));
    ui->setupUi(this);
    m_bg->resize(this->size());

    m_info = info;
    m_name = name;
    m_mark = mark;
    InitWindow();
}

MyTalkWidget::~MyTalkWidget()
{
    delete ui;
}

void MyTalkWidget::SendMsg()
{
    qDebug("send msg\n");
    std::string msg;
    std::string who;
    msg += "<font size=5>";
    msg += ui->textEdit_send_text->toPlainText().toStdString();
    msg += "</font>";

    who += "<font size=8><font color=darkgray>";
    who += m_name;
    who += ":";
    who += "</font></font>";

    if(!msg.empty())
    {
        ui->textEdit_text->setAlignment(Qt::AlignRight);
        ui->textEdit_text->append(who.c_str());
        ui->textEdit_text->setAlignment(Qt::AlignRight);
        ui->textEdit_text->append(msg.c_str());
        ui->textEdit_send_text->clear();
#if 0// TODO send msg to server
        MyTalkClient::SendMsg(...);
#endif
    }
}

void MyTalkWidget::InitWindow()
{
    ui->label_photo->setText("<img src=../photo.png></img>");
    std::string name;
    name += "<font size=8>";
    name += m_name;
    name += "</font>";
    ui->label_name->setText(name.c_str());
    std::string title;
    title += "talk with ";
    title += m_name;
    title += "...";
    this->setWindowTitle(title.c_str());

    connect(ui->pushButton_close,SIGNAL(clicked(bool)),this,SLOT(hide()));
    connect(ui->pushButton_send,SIGNAL(clicked(bool)),this,SLOT(SendMsg()));
    this->hide();
}

void MyTalkWidget::Update()
{
    std::string name;
    name += "<font size=8>";
    name += m_name;
    name += "</font>";
    ui->label_name->setText(name.c_str());
    std::string title;
    title += "talk with ";
    title += m_name;
    title += "...";
    this->setWindowTitle(title.c_str());
}
