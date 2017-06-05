#include "MainWidget.h"
#include <QApplication>
#include "MyRecvCap.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MyAddrInfo info;
    info.SetIpAndPort("127.0.0.1",4399);
    MyRecvCap recvCap(info,"recvcap");

    MainWidget w("mainwidget");
    w.show();

    return a.exec();
}
