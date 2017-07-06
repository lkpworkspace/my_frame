#include "MainWidget.h"
#include <QApplication>
#include "MyCapClient.h"
#include "MyLogin.h"
#include <QFont>

#if 1
#define SERVER_IP "115.159.143.187"
#else
#define SERVER_IP "127.0.0.1"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font  = a.font();
    font.setPointSize(14);
    a.setFont(font);

    MyCapClient* client = new MyCapClient("client",SERVER_IP,4399);

    MainWidget w("mainwidget");
    w.show();
#if 1
    client->Connect();
    client->Start();
#endif
    return a.exec();
}
