#include "MyMainWidget.h"
#include "ui_MyMainWidget.h"
#include "MyPeopleList.h"
#include <QListWidget>
#include "MyUdp.h"
MyMainWidget::MyMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyMainWidget)
{
    this->AddBg();
    ui->setupUi(this);
    m_normal = new MyNormalEvent();

    InitWindow();
}

MyMainWidget::~MyMainWidget()
{
    delete ui;
}



void MyMainWidget::PushBegin()
{
    m_normal->Work();
    MyAddrInfo info;
    MyTalkWidget* x = new MyTalkWidget(info,"kpli","kpli","easy coding on linux");
    x->show();
}

void MyMainWidget::AddPeople()
{
#if 0
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setAutoScroll(true);
    QListWidgetItem* item = new QListWidgetItem();

    QWidget *wContainer = new QWidget(ui->listWidget);
    QHBoxLayout *hLayout = new QHBoxLayout(wContainer);

    QLabel *alarmIcon = new QLabel(tr("beih"));
    QLabel *placeLabel = new QLabel(tr("北京"));
    QLabel *videoNumLabel = new QLabel(tr("8"));
    QLabel *dateLabel = new QLabel(tr("2013-4-16"));
    QLabel *alarmMsgLabel = new QLabel(tr("违章搭建"));

    hLayout->addWidget(alarmIcon);
    hLayout->addStretch(1);
    hLayout->addWidget(placeLabel);
    hLayout->addStretch(1);
    hLayout->addWidget(videoNumLabel);
    hLayout->addStretch(1);
    hLayout->addWidget(dateLabel);
    hLayout->addStretch(1);
    hLayout->addWidget(alarmMsgLabel);
    hLayout->setContentsMargins(5,0,0,5); //关键代码，如果没有很可能显示不出来
    wContainer->resize(350,50);

    ui->listWidget->setItemWidget(item,wContainer);
#else

#endif
}

void MyMainWidget::InitWindow()
{
    m_bg->resize(this->size());
    ui->label_photo->setPixmap(QPixmap("../my.png"));

    // TODO...
    // test
    m_people_list = new MyPersonList();
    ui->vLayout->addWidget(m_people_list);

    // add person
    m_people_list->slotAddGroup();
    m_people_list->slotAddBuddy();
    m_people_list->slotAddBuddy();
    m_people_list->slotAddGroup();
    m_people_list->slotAddBuddy();
    m_people_list->slotAddBuddy();

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(PushBegin()));
}

void MyMainWidget::AddBg()
{
    m_bg = new QLabel(this);
    m_bg->setPixmap(QPixmap("../bg.jpg"));
    m_bg->resize(this->size());
}
