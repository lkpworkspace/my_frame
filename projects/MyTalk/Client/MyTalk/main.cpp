#include "MyMainWidget.h"
#include <QApplication>
#include "MyFrame.h"
#include "MyUdp.h"
using namespace my_master;
MyUdp *g_udp = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::thread thr([](){
        MyApp app(4,1024);
        g_udp = new MyUdp("",4399,false);
        g_udp->Bind();
        app.AddEvent(g_udp);
        app.Exec();
    });
    thr.detach();

    MyMainWidget w;
    w.show();

    return a.exec();
}
