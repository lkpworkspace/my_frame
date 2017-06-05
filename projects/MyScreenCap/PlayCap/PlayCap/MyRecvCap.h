#ifndef MYRECVCAP_H
#define MYRECVCAP_H

#include <QObject>
#include "MyControls.h"
#include "MyFrame.h"
#include "MyUdp.h"

using namespace my_master;

class MyRecvCap : public QObject, public MyCtrlObj
{
    Q_OBJECT
public:
    explicit MyRecvCap(MyAddrInfo info,std::string key,QObject *parent = 0);

signals:

public slots:

private:
    void PlayCap();
    MyUdp* m_udp;
    MyAddrInfo m_info;
};

#endif // MYRECVCAP_H
