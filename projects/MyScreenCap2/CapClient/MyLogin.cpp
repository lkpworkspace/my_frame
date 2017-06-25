#include "MyLogin.h"
#include <QRect>
#include <QDebug>
#include <QMessageBox>

MyLogin::MyLogin(QWidget *parent) : QWidget(parent)
{
#if 0
    //add background picture
    QLabel *background = new QLabel("<img src=../bg.jpg></img>");
    background->setGeometry(QRect(0,0,this->size().width(),120));
    background->setParent(this);
#endif
    QGridLayout *lay = new QGridLayout(this);
    QHBoxLayout *lay1 = new QHBoxLayout;
    QHBoxLayout *lay2 = new QHBoxLayout;
    QHBoxLayout *lay3 = new QHBoxLayout;
    lay->setRowStretch(0,1);
    lay->addLayout(lay1,1,0);
    lay->addLayout(lay2,2,0);
    lay->addLayout(lay3,3,0);

    lay1->addWidget(new QLabel("<font size=15>账号</font>"));
    lay2->addWidget(new QLabel("<font size=15>密码</font>"));
    lay1->addWidget(m_account = new QLineEdit);
    lay2->addWidget(m_password = new QLineEdit);
    m_password->setEchoMode(QLineEdit::Password);
    lay3->stretch(1);
    lay3->addWidget(m_login = new QPushButton("连接"));
    connect(m_login,SIGNAL(clicked(bool)),this,SLOT(slotLoginButtonClicked(bool)));
    lay3->addSpacing(20);
    lay3->addWidget(m_cancel = new QPushButton("取消"));
    connect(m_cancel,SIGNAL(clicked(bool)),this,SLOT(close()));

    this->setFixedSize(400,300);
    this->setWindowTitle("MyTalk Login");
}

//点击登录按钮触发此函数
void MyLogin::slotLoginButtonClicked(bool)
{
    qDebug("login clicked");
    QString account = m_account->text();
    QString pass = m_password->text();
    emit Login(account, pass);
    hide();
}
