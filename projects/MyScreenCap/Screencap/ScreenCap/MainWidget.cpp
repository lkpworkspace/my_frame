#include "MainWidget.h"
#include "ui_mainwidget.h"
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include "MySendCap.h"
#include <QDebug>

MainWidget::MainWidget(std::__cxx11::string key, QWidget *parent) :
    QWidget(parent),
    MyCtrlObj(key),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    // init
    m_screen = QGuiApplication::primaryScreen();
    m_screencap_timer = new QTimer(this);

    // connect signal and slot
    connect(ui->pushButton_begin,SIGNAL(clicked(bool)),this,SLOT(ButtonStartStop()));
    connect(m_screencap_timer,SIGNAL(timeout()),this,SLOT(BeginCap()));

    // init method
    ui->pushButton_begin->setText("Start");
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
    cap.save(&buf,"png",20);

    // send to play client
    MySendCap* sc = (MySendCap*)CALL_CTRL("sendcap");
    qDebug("%d",ba.size());
    if(sc != NULL)
        sc->Send(ba.data(),ba.size());
#if 0
    // load pic from bytearray
    QPixmap temp;
    temp.loadFromData(ba,"png");

    // display to window
    ui->label->setPixmap(temp);
    ui->label->setScaledContents(true);
#endif
}

void MainWidget::ButtonStartStop()
{
    if(m_is_start)
        StopCap();
    else
        StartCap();
    m_is_start = !m_is_start;
}

void MainWidget::StartCap()
{
    m_screencap_timer->start(20);
    ui->pushButton_begin->setText("Stop");
}

void MainWidget::StopCap()
{
    ui->pushButton_begin->setText("Start");
    m_screencap_timer->stop();
}
