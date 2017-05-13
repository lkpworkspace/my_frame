#include "MyMainWidget.h"
#include <QApplication>
#include "MyFrame.h"
#include "MyUdp.h"
#include "MyLogin.h"
#include "MyTalkClient.h"
#include <thread>

using namespace my_master;
MyUdp *g_udp = nullptr;
MyMainWidget *g_main_widget = nullptr;
#if 0
#include "MyPeopleItem.h"
#include "MyTalkWidget.h"
#endif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // init client
    MyTalkClient::Init();

    // init my frame
    std::thread thr([](){
        MyApp app(1,1024);

        // my talk udp
        g_udp = new MyUdp("",0,false);
        //g_udp->Bind();
        app.AddEvent(g_udp);

        MyTalkClient* client = new MyTalkClient();
        app.Exec();
    });
    thr.detach();

    // load qss
    QFile styleSheet(":/myqss.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
        return -1;
    }
    qApp->setStyleSheet(styleSheet.readAll());

#if 0
    MyAddrInfo info;
    MyTalkWidget w(info,"kpli","easy coding on linux");
    w.show();
#endif
#if 1
    // show login
    MyLogin login;
    login.show();
#endif
#if 0
    MyMainWidget w;
    w.show();
#endif
#if 0
    MyPeopleItem item;
    item.show();
#endif
    return a.exec();
}
