#include "MainWidget.h"
#include <QApplication>
#include "MySendCap.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w("mainwidget");
    w.show();
#if 1
    MyAddrInfo info;
    info.SetIpAndPort("127.0.0.1",4399);
    MySendCap sendcap("sendcap",info);
#endif
    return a.exec();
}
