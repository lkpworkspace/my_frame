#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "MyControls.h"
#include "MyCapClient.h"
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>
#include <QSize>

MainWidget::MainWidget(std::__cxx11::string key, QWidget *parent) :
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

    // init method
    ui->pushButton_begin->setText("Connect...");
    m_is_start = false;
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
#if 0
    // send to play client
    MySendCap* sc = (MySendCap*)CALL_CTRL("sendcap");
    qDebug("%d",ba.size());
    if(sc != NULL)
        sc->Send(ba.data(),ba.size());
#endif
#if 1
    // load pic from bytearray
    QPixmap temp;
    temp.loadFromData(ba,"png");

    // display to window
    ui->label_pic->setPixmap(temp);
    ui->label_pic->setScaledContents(true);
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


void MainWidget::ButtonStartStop()
{
    m_login->show();
    ((MyCapClient*)REQUEST("client"))->ConnectTo("123456789","1234");
#if 0
    if(m_is_start)
        StopCap();
    else
        StartCap();
    m_is_start = !m_is_start;
#endif
}

void MainWidget::StartCap()
{
    m_screencap_timer->start(20);
    ui->textEdit->append("begin capture...");
}

void MainWidget::StopCap()
{
    m_screencap_timer->stop();
    ui->textEdit->append("stop capture...");
}
