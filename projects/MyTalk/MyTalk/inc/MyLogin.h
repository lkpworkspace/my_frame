#ifndef MYLOGIN_H
#define MYLOGIN_H

#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QString>
#include "MyMainWidget.h"

class MyLogin : public QWidget
{
    Q_OBJECT
public:
    explicit MyLogin(QWidget *parent = 0);

    QLineEdit *m_account;
    QLineEdit *m_password;
    QPushButton *m_login;
    QPushButton *m_cancel;
signals:

public slots:
    void slotLoginButtonClicked(bool);
};

#endif // MYLOGIN_H
