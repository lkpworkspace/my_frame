#ifndef MYRECVCAP_H
#define MYRECVCAP_H

#include <QObject>
#include <QByteArray>
#include "MyControls.h"
#include "MyFrame.h"
#include "MyUdp.h"

using namespace my_master;

class MyRecvCap : public QObject, public MyCtrlObj, public my_master::MyThread
{
    Q_OBJECT
public:
    explicit MyRecvCap(MyAddrInfo info,std::string key,QObject *parent = 0);

    void Run();

signals:
    void GetPic(QByteArray);
public slots:
    void RecvStop();
private:
    char* m_buf;
    int m_index;

    MyUdp* m_udp;
    MyAddrInfo m_info;
};

#endif // MYRECVCAP_H
