#include "../inc/MyBufItem.h"

#define _C(X) QString::fromLocal8Bit(X)


MyBufItem::MyBufItem(QWidget *parent)
    :QWidget(parent)
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

    connect(m_butDel,SIGNAL(clicked()),this,SLOT(OnButClicked()));
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
    }
    return buf;
}

void MyBufItem::OnButClicked()
{
    emit DelMySelf();
}

void MyBufItem::AddComboBoxItem(const QString &str)
{
    m_comBox->addItem(str);
}










