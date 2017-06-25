#include "MainWidget.h"
#include <QApplication>
#include "MyCapClient.h"
#include "MyLogin.h"
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font  = a.font();
    font.setPointSize(14);
    a.setFont(font);

    MyCapClient* client = new MyCapClient("client","127.0.0.1",4399);

    MainWidget w("mainwidget");
    w.show();
#if 1
    client->Connect();
    client->Start();
#endif
    return a.exec();
}
