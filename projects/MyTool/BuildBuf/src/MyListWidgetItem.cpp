#include "../inc/MyListWidgetItem.h"
#include <QDateTime>

MyListWidgetItem::MyListWidgetItem(const char* buf, int len)
    :QListWidgetItem("Loading...")
{
    QString str1;
    QString text = "[";
    text += QDateTime::currentDateTime().toString("hh:mm:ss");
    text += "]: ";
    for(int i = 0; i < len; ++i)
    {
        if((uchar)buf[i] >= 32 && (uchar)buf[i] < 127)
            str1 = str1.sprintf("%c",(uchar)buf[i]);
        else
            str1 = str1.sprintf("%c", 46);
        text += str1;
    }

    this->setText(text);
    m_buf = NULL;
    m_len = 0;

    SetBuf(buf,len);
}

MyListWidgetItem::~MyListWidgetItem()
{
    if(m_buf != NULL)
        delete[] m_buf;
    m_buf = NULL;
}

void MyListWidgetItem::SetBuf(const char *buf, int len)
{
    if(m_buf != NULL)
        delete[] m_buf;
    m_len = len;
    m_buf = new char[len];
    memcpy(m_buf,buf,len);
}

char* MyListWidgetItem::GetBuf(int &len)
{
    len = m_len;
    return m_buf;
}
