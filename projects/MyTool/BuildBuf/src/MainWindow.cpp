#include "../inc/MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUI();
    m_isOpen = false;
    m_client = new QTcpSocket(this);

    connect(m_client,SIGNAL(readyRead()),this,SLOT(ReadMessage()));
    connect(m_client,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_client;
    m_client = NULL;
}

void MainWindow::InitUI()
{
    // init ComBox
    ui->comboBox_Protocol->addItem("TCP");
    ui->comboBox_Protocol->addItem("UDP");

    // init scroll area
    QWidget* w = new QWidget();
    m_scrollWidget = w;
    m_buflayout = new QVBoxLayout();
    m_buflayout->addStretch();
    w->setLayout(m_buflayout);
    ui->scrollArea->setWidget(w);
    connect(ui->pushButton_Add,&QPushButton::clicked,[=](){
       MyBufItem* item = new MyBufItem();
       m_buflayout->insertWidget(m_buflayout->count() - 1,item);
       connect(item,&MyBufItem::DelMySelf,[=](){
            m_buflayout->removeWidget(item);
            delete item;
       });
    });

    // init lineedit
    ui->lineEdit_Port->setValidator(new QIntValidator(ui->lineEdit_Port));
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_Port->setText("4399");
    ui->pushButton_Send->setEnabled(false);

    ui->mainToolBar->hide();
}

char* MainWindow::GetBuildBuf(int& len)
{
    int count = 0;
    char* buf = NULL;
    int index = 0;

    for(int i = 0; i < m_buflayout->count() - 1; ++i)
    {
        MyBufItem* item = (MyBufItem*)(m_buflayout->itemAt(i)->widget());
        count += item->GetLen();
    }
    if(count <= 0) return NULL;
    buf = new char[count];

    for(int i = 0; i < m_buflayout->count() - 1; ++i)
    {
        int templen = 0;
        MyBufItem* item = (MyBufItem*)(m_buflayout->itemAt(i)->widget());
        char* temp = item->GetBuf(templen);
        memcpy(&buf[index],temp,templen);
        index += templen;
    }
    len = count;
    return buf;
}

void MainWindow::ShowBuf(const char *buf, int len)
{
    if(buf == NULL)
    {
        qDebug() << "[MainWindow::ShowBuf] : empty buf";
        return;
    }
    int ReadSize = len;
    uchar *pdata;
    pdata = (uchar*)buf;

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(ReadSize/0x10 + 1);
    ui->tableWidget->setColumnCount(0x11);
    QStringList str1;
    int i, j;
    QString str,str2,strtext;
    for(i = 0; i < 0x10; ++i)
    {
        str1 << QString::number(i,16);
        ui->tableWidget->setColumnWidth(i,40);
    }
    ui->tableWidget->setColumnWidth(0x10,250);
    str1 << "TEXT";
    ui->tableWidget->setHorizontalHeaderLabels(str1);
    str1.clear();

    for(i = 0x0; i < ReadSize/0x10+1; ++i)
    {
        str = str.sprintf("%08x",i*0x10);
        str1 << str;
    }
    ui->tableWidget->setVerticalHeaderLabels(str1);

    int index = 0;
    QTableWidgetItem* item;
    for(i = 0; i < ReadSize/0x10 + 1; ++i)
    {
        strtext.clear();
        for(j = 0; j < 0x10; ++j)
        {
            index = i*0x10 +j;
            if(index >= len) break;
            str = str.sprintf("%02x",pdata[index]);
            if((uchar)pdata[index] >= 32 && (uchar)pdata[index] < 127)
                str2 = str2.sprintf("%c",(uchar)pdata[index]);
            else
                str2 = str2.sprintf(".");
            strtext += str2;
            ui->tableWidget->setItem(i,j,item = new QTableWidgetItem(str));
            item->setTextAlignment(Qt::AlignCenter);
        }
        ui->tableWidget->setItem(i,0x10,item = new QTableWidgetItem(strtext));
        item->setTextAlignment(Qt::AlignCenter);
    }
}

void MainWindow::NewConnect(QString IP, int port)
{
    m_blocksize = 0;
    m_client->abort();;
    m_client->connectToHost(IP,port);
}

void MainWindow::ReadMessage()
{
    ui->textEdit_Tips->append("Get Msg");

    m_blocksize = m_client->bytesAvailable();
    m_message = m_client->readAll();

    MyListWidgetItem* item = new MyListWidgetItem(m_message.data(),m_message.count());
    ui->listWidget_Recv->addItem(item);
}

void MainWindow::DisplayError(QAbstractSocket::SocketError)
{
    qDebug() <<m_client->errorString();
    ui->textEdit_Tips->append(m_client->errorString());
    if(m_client->error() == QAbstractSocket::RemoteHostClosedError ||
            m_client->error() == QAbstractSocket::ConnectionRefusedError)
        on_pushButton_Connect_clicked();
}

void MainWindow::on_listWidget_Send_itemClicked(QListWidgetItem *item)
{
    MyListWidgetItem* it = (MyListWidgetItem*)item;
    int len;
    char* buf = it->GetBuf(len);
    ShowBuf(buf,len);
}

void MainWindow::on_listWidget_Recv_itemClicked(QListWidgetItem *item)
{
    MyListWidgetItem* it = (MyListWidgetItem*)item;
    int len;
    char* buf = it->GetBuf(len);
    ShowBuf(buf,len);
}

void MainWindow::on_pushButton_Send_clicked()
{
    char* buf = NULL;
    int len = 0;

    buf = GetBuildBuf(len);
    if(buf == NULL) return;
    // add to ui
    MyListWidgetItem* item = new MyListWidgetItem(buf,len);
    ui->listWidget_Send->addItem(item);
    ui->textEdit_Tips->append(_C("Send ") + QString::number(len) + _C(" bytes data"));

    if(ui->comboBox_Protocol->currentText() == "TCP")
    {
        m_client->write(buf,len);
        ShowBuf(buf,len);
    }
    delete[] buf;
}

void MainWindow::on_pushButton_View_clicked()
{
    char* buf = NULL;
    int len = 0;

    buf = GetBuildBuf(len);
    ui->label_BufCount->setText(QString::number(len));
    if(buf == NULL) return;

    ShowBuf(buf,len);
    delete[] buf;
}

void MainWindow::on_pushButton_Connect_clicked()
{
    QString IP = ui->lineEdit_IP->text();
    QString port = ui->lineEdit_Port->text();

    IP = IP.trimmed();
    port = port.trimmed();

    QString str = "^[0-9]{1,3}(?:\.[0-9]{1,3}){3}$";
    QRegExp rx(str);
    if(rx.indexIn(ui->lineEdit_IP->text()) == 0)
    {
        qDebug() << "reg okok";
        qDebug() << rx.capturedTexts();
    }else
    {
        ui->textEdit_Tips->append(_C("Err format"));
        return;
    }
    if(m_isOpen)
    {
        ui->pushButton_Connect->setText(_C("Connect"));
        m_client->close();
        ui->pushButton_Send->setEnabled(false);
    }else
    {
        short p = port.toShort();
        NewConnect(IP,p);
        ui->pushButton_Connect->setText(_C("Disconnect"));
        ui->pushButton_Send->setEnabled(true);
    }
    m_isOpen = !m_isOpen;
}

void MainWindow::on_comboBox_Protocol_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->textEdit_Tips->append(_C("use TCP"));
        break;
    case 1:
        ui->textEdit_Tips->append(_C("use UDP"));
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_Save_clicked()
{
    QMessageBox::information(this,"Save",_C("Save buf?"));
}
