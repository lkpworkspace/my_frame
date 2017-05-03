#include "MyPeopleItem.h"
#include <QPainter>
#include <QMouseEvent>

MyPeopleItem::MyPeopleItem(QWidget *parent) :
    QWidget(parent)
{
    initUi();
    m_talk = new MyTalkWidget(m_info, "kpli", "kpli","easy coding on linux");
    m_talk->hide();
    headPath="../photo.png";
}

void MyPeopleItem::SetAddr(MyAddrInfo info)
{
    m_info = info;
    m_talk->SetAddr(info);
}

void MyPeopleItem::SetMark(std::string mark)
{
    this->sign->setText(mark.c_str());
    m_talk->SetMark(mark);
}

void MyPeopleItem::SetName(std::string name)
{
    std::string temp;
    temp += name;
    temp += "(";
    temp += m_info.GetIp();
    temp += ":";
    temp += std::to_string(m_info.GetPort());
    temp += ")";
    this->name->setText(temp.c_str());
    m_talk->SetName(name);
}


//初始化Ui
void MyPeopleItem::initUi()
{
    //初始化
    head=new QWidget(this);
    name=new QLabel(this);
    sign=new QLabel(this);
    //设置头像大小
    head->setFixedSize(40,40);
    //设置个性签名字体为灰色
    QPalette color;
    color.setColor(QPalette::Text,Qt::gray);
    sign->setPalette(color);
    //布局
    head->move(7,7);
    name->move(54,10);
    sign->move(54,27);
    //装载事件过滤器
    head->installEventFilter(this);
}
//事件过滤器，主要是为了让图片能够全部填充在head里面
bool MyPeopleItem::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == head)
    {
        if(event->type() == QEvent::Paint)
        {
            QPainter painter(head);
            painter.drawPixmap(head->rect(), QPixmap(headPath));
        }
    }
    return QWidget::eventFilter(obj, event);
}



void MyPeopleItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_talk->show();
    }
}






















