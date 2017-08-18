#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QObject>
#include <string>
#include <QString>
#include "MyNet.h"

using namespace my_master;
class MyDelegate : public QObject
{
    Q_OBJECT
public:
    static MyDelegate* GetInstance();
    void AppendMsg(std::string,std::string);
    void AddFriend(MyAddrInfo info,
                   std::string name,
                   std::string account,
                   std::string mark);
signals:
    void appendMsg(QString,QString);
    void addFriend(QString,
                   unsigned short,
                   QString,
                   QString,
                   QString);
public slots:
private:
    explicit MyDelegate(QObject *parent = 0);
    static MyDelegate* instance;
};

#endif // MYDELEGATE_H
