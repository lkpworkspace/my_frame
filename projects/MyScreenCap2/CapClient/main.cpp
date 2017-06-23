#include "MainWidget.h"
#include <QApplication>
#include "MyCapClient.h"
#include "MyLogin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MyCapClient* client = new MyCapClient("client","127.0.0.1",4399);


    MainWidget w("mainwidget");
    w.show();

    client->Connect();
    client->Start();

    return a.exec();
}
