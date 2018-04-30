#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QDebug>
#include "../inc/MyBufItem.h"
#include "../inc/MyListWidgetItem.h"
#include <QTcpSocket>
#include <QUdpSocket>
#include <QtNetwork>
#include <QTableWidget>
#include <QMessageBox>

#define MaxData(x,y) (((x) > (y))?(x):(y))
#define MinData(x,y) (((x) < (y))?(x):(y))

#define _C(X) QString::fromLocal8Bit(X)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_listWidget_Send_itemClicked(QListWidgetItem *item);

    void on_listWidget_Recv_itemClicked(QListWidgetItem *item);

    void on_pushButton_Send_clicked();

    void on_pushButton_View_clicked();

    void on_pushButton_Connect_clicked();

    void on_comboBox_Protocol_currentIndexChanged(int index);

    void on_pushButton_Save_clicked();

    void NewConnect(QString IP, int port);
    void ReadMessage();
    void DisplayError(QAbstractSocket::SocketError);
    void on_checkBox_SendLoop_clicked(bool checked);

    void SendLoop();
private:
    void InitUI();
    void ShowBuf(const char* buf, int len);
    void LoadBuildBuf();
    char* GetBuildBuf(int& len);

    Ui::MainWindow *ui;
    QVBoxLayout* m_buflayout;
    QTcpSocket* m_client;
    bool m_isOpen;
    quint16 m_blocksize;
    QByteArray m_message;
    QWidget* m_scrollWidget;
    QTimer* m_sendLoop;
};

#endif // MAINWINDOW_H
