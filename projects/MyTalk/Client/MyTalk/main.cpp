#include "MyMainWidget.h"
#include <QApplication>
#include "MyFrame.h"
using namespace my_master;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::thread thr([](){
        MyApp app(4,1024);
        app.Exec();
    });
    thr.detach();

    MyMainWidget w;
    w.show();

    return a.exec();
}
