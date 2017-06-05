#include "MainWidget.h"
#include <QApplication>
#include "MyRecvCap.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.show();

    MyAddrInfo info;
    info.SetIpAndPort("127.0.0.1",4399);
    MyRecvCap recvCap(info,"recvcap");

    return a.exec();
}
