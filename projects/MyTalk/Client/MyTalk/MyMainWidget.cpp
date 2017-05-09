#include "MyMainWidget.h"
#include "ui_MyMainWidget.h"
#include <QListWidget>
#include "MyUdp.h"
#include "MyTalkClient.h"

MyMainWidget::MyMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyMainWidget)
{
    this->AddBg();
    ui->setupUi(this);
    //m_normal = new MyNormalEvent();
    InitWindow();
}

MyMainWidget::~MyMainWidget()
{
    delete ui;
}

//void MyMainWidget::slotTimeout()
//{

//    MyTalkClient::m_mutex.lock();
//    for(auto it=MyTalkClient::m_friends.begin();it!=MyTalkClient::m_friends.end();++it)
//    {
//        mytalk_friend_t* myfriend = it->second;
//        //delete it->second;
//        //MyTalkClient::m_friends.erase(it);
//        AddFriend(myfriend->info,myfriend->name,myfriend->account,myfriend->mark);
//    }
//    MyTalkClient::m_mutex.unlock();
//    m_check_friend_timer->stop();
//}

//void MyMainWidget::PushBegin()
//{
//    //m_normal->Work();
//    MyAddrInfo info;
//    MyTalkWidget* x = new MyTalkWidget(info,"kpli","kpli","easy coding on linux");
//    x->show();
//}

void MyMainWidget::AppendMsg(QString account, QString msg)
{
    printf("signal is coming %s\n",msg.toStdString().c_str());
    m_people_list->GetTalkWidget(account.toStdString())->AppendMsg(msg.toStdString());
}

void MyMainWidget::AddMyFriend(QString ip,
                 unsigned short port,
                 QString name,
                 QString account,
                 QString mark)
{
    printf("signal is coming...\n");
    MyAddrInfo info;
    info.SetIpAndPort(ip.toStdString(),port);
    AddFriend(info,name.toStdString(),account.toStdString(),mark.toStdString());
}

void MyMainWidget::InitWindow()
{
    m_bg->resize(this->size());
    ui->label_photo->setPixmap(QPixmap("../my.png"));
    this->setWindowTitle("MyTalk");
    ui->label_info->setText(MyTalkClient::m_account.c_str());

    m_people_list = new MyPersonList();
    ui->vLayout->addWidget(m_people_list);

    // add person group
    m_people_list->slotAddGroup();

    // signal and slot
    // connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(PushBegin()));
    connect(MyDelegate::GetInstance(),SIGNAL(appendMsg(QString,QString)),this,SLOT(AppendMsg(QString,QString)));
    connect(MyDelegate::GetInstance(),SIGNAL(addFriend(QString,unsigned short,QString,QString,QString)),this,SLOT(AddMyFriend(QString,unsigned short,QString,QString,QString)));
}

void MyMainWidget::AddBg()
{
    m_bg = new QLabel(this);
    m_bg->setPixmap(QPixmap("../bg.jpg"));
    m_bg->resize(this->size());
}
