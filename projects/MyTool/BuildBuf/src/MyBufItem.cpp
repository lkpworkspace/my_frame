#include "../inc/MyBufItem.h"
#include <QDebug>

#define _C(X) QString::fromLocal8Bit(X)


MyBufItem::MyBufItem(QWidget *parent)
    :QWidget(parent),
      m_buf(NULL)
{
    QHBoxLayout* hbox = new QHBoxLayout(this);

    m_butDel = new QPushButton(_C("Del"));
    m_butDel->setMinimumHeight(20);
    m_butDel->setMinimumWidth(40);
    m_butDel->setFont(QFont("x",11));
    m_comBox = new QComboBox();
    m_comBox->setMinimumHeight(20);
    m_comBox->setMinimumWidth(60);
    m_comBox->setFont(QFont("x",11));
    m_edit = new QLineEdit();
    m_edit->setFont(QFont("x",11));
    m_edit->setMinimumHeight(25);
    m_edit->setMinimumWidth(170);

    hbox->addWidget(m_edit);
    hbox->addWidget(m_comBox);
    hbox->addWidget(m_butDel);

    this->setLayout(hbox);
    this->setMinimumHeight(50);

    // all num
    AddComboBoxItem("char");
    AddComboBoxItem("hex ch");
    AddComboBoxItem("short");
    AddComboBoxItem("hex sh");
    AddComboBoxItem("int");
    AddComboBoxItem("hex in");
    AddComboBoxItem("flaot");
    AddComboBoxItem("string");
    AddComboBoxItem("hex dat");

    connect(m_butDel,SIGNAL(clicked()),this,SLOT(OnButClicked()));
}

MyBufItem::~MyBufItem()
{
    if(m_buf != NULL)
        delete[] m_buf;
    m_buf = NULL;
}


int MyBufItem::GetLen()
{
    int len=0;
    switch(m_comBox->currentIndex())
    {
    case 0:
    case 1:
        len=1;
        break;
    case 2:
    case 3:
        len=2;
        break;
    case 4:
    case 5:
    case 6:
        len=4;
        break;
    case 7:
        len=m_edit->text().toLatin1().count();
        break;
    case 8:
        len = m_edit->text().split(' ').size();
    }
    return len;
}

char* MyBufItem::GetBuf(int &len)
{
    char* buf = NULL;
    char ch;
    short sh;
    int in;
    float fl;
    switch(m_comBox->currentIndex())
    {
    case 0:
        len = 1;
        buf = new char[1];
        ch = m_edit->text().toInt();
        memcpy(buf,&ch,1);
        break;
    case 1:
        len = 1;
        buf = new char[1];
        ch = m_edit->text().toInt(0,16);
        memcpy(buf,&ch,1);
        break;
    case 2:
        len = 2;
        buf = new char[2];
        sh = m_edit->text().toInt();
        memcpy(buf,&sh,2);
        break;
    case 3:
        len = 2;
        buf = new char[2];
        sh = m_edit->text().toInt(0,16);
        memcpy(buf,&sh,2);
        break;
    case 4:
        len = 4;
        buf = new char[4];
        in = m_edit->text().toInt();
        memcpy(buf,&in,4);
        break;
    case 5:
        len = 4;
        buf = new char[4];
        sh = m_edit->text().toInt(0,16);
        memcpy(buf,&in,4);
        break;
    case 6:
        len = 4;
        buf = new char[4];
        sh = m_edit->text().toFloat();
        memcpy(buf,&fl,4);
        break;
    case 7:
        len = m_edit->text().toLatin1().count() + 1;
        buf = new char[len];
        memset(buf,0,len);
        memcpy(buf,m_edit->text().toLatin1().data(),len  - 1);
        len -= 1;
        break;
    case 8:
    {
        QStringList list = m_edit->text().split(' ');
        len = list.size();
        buf = new char[len];
        memset(buf,0,len);
        for(int i = 0; i < list.size(); ++i)
        {
            buf[i] = (char)list.at(i).toShort(NULL,16);
        }
    }
        break;
    }
    m_buf = buf;
    return buf;
}

char* MyBufItem::ToData(QByteArray a, int& outlen)
{
    QByteArray temp_array = a.toHex();
    qDebug() << temp_array;
    outlen = temp_array.count();
    char* temp_buf = new char[outlen];
    memset(temp_buf,0,outlen);
    memcpy(temp_buf,temp_array.data(),outlen);
    printf("\n");
    for(int i = 0; i < outlen; ++i)
    {
        printf("%02X\t",temp_buf[i]);
    }
    printf("\n");
    return temp_buf;
}

void MyBufItem::OnButClicked()
{
    emit DelMySelf();
}

void MyBufItem::AddComboBoxItem(const QString &str)
{
    m_comBox->addItem(str);
}










