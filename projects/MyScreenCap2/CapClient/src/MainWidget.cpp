#include "../inc/MainWidget.h"
#include "../bin/ui_MainWidget.h"
#include "MyControls.h"
#include "../inc/MyCapClient.h"
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>
#include <QSize>

MainWidget::MainWidget(std::string key, QWidget *parent) :
    QWidget(parent),
    MyCtrlObj(key),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    // init
    m_login = new MyLogin();
    m_login->hide();
    m_screen = QGuiApplication::primaryScreen();
    m_screencap_timer = new QTimer(this);

    // connect signal and slot
    connect(ui->pushButton_begin,SIGNAL(clicked(bool)),this,SLOT(ButtonStartStop()));
    connect(m_screencap_timer,SIGNAL(timeout()),this,SLOT(BeginCap()));
    connect(this,SIGNAL(GetAccount()),this,SLOT(UpdateAccount()));
    connect(this,SIGNAL(Cap()),this,SLOT(StartCap()));
    connect(this,SIGNAL(EndCap()),this,SLOT(StopCap()));
    connect(m_login,SIGNAL(Login(QString,QString)),this,SLOT(GetAccAndPass(QString,QString)));
    connect(this,SIGNAL(GetPic(QByteArray)),this,SLOT(DisplayPic(QByteArray)));

    // init method
    ui->pushButton_begin->setText("Connect...");
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::BeginCap()
{
    QByteArray ba;
    QBuffer buf(&ba);

    QPixmap cap = m_screen->grabWindow(0);
    // save to array
    buf.open(QIODevice::WriteOnly);
    // scale
    QSize picSize(600,350);
    QPixmap scaledPixmap = cap.scaled(picSize);
    scaledPixmap.save(&buf,"png",20);
#if 1
    // send to play client
    MyCapClient* sc = (MyCapClient*)REQUEST("client");
    //qDebug("pic size %d",ba.size());
    if(sc != NULL)
        sc->EasySendPic(ba.data(),ba.size());
#endif
#if 0
    // load pic from bytearray
    QPixmap temp;
    temp.loadFromData(ba,"png");

    // display to window
    ui->label_pic->setPixmap(temp);
    ui->label_pic->setScaledContents(true);
#endif
#if 0
    // test
    for(int i = 0; i < 20; ++i)
        printf("%02x\n",ba.at(i));
    m_screencap_timer->stop();
#endif
    ba.clear();
}

void MainWidget::UpdateAccount()
{
    MyCapClient* client = ((MyCapClient*)REQUEST("client"));
    ui->textEdit->append("connect ok...");
    ui->label_account->setText(client->GetAccount().c_str());
    ui->label_pass->setText(client->GetPass().c_str());
}

void MainWidget::GetAccAndPass(QString acc,QString pass)
{
    qDebug() << acc << "  " << pass;
    ((MyCapClient*)REQUEST("client"))->ConnectTo(acc.toStdString(),pass.toStdString());
}

void MainWidget::DisplayPic(QByteArray ba)
{
    // load pic from bytearray
#if 1
    QPixmap temp;
    temp.loadFromData(ba,"png");

    // display to window
    ui->label_pic->setPixmap(temp);
    ui->label_pic->setScaledContents(true);
#else
    qDebug("dis pic size %d\n",ba.size());
    for(int i = 0; i < 20; ++i)
        printf("%02x\n",ba.at(i));

    static int temp = 0;
    ui->label_pic->setText(std::to_string(temp++).c_str());
#endif
}


void MainWidget::ButtonStartStop()
{
    m_login->show();
}

void MainWidget::StartCap()
{
    m_screencap_timer->start(20);
    ui->textEdit->append("begin capture...");
    ui->label_pic->setText("");
}

void MainWidget::StopCap()
{
    m_screencap_timer->stop();
    ui->textEdit->append("stop capture...");
    ui->label_pic->setText("connect client quit");
}
