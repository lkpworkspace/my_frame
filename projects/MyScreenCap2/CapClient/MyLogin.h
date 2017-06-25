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

class MyLogin : public QWidget
{
    Q_OBJECT
public:
    explicit MyLogin(QWidget *parent = 0);

    std::string GetAccount();
    std::string GetPass();
    QLineEdit *m_account;
    QLineEdit *m_password;
    QPushButton *m_login;
    QPushButton *m_cancel;
signals:
    void Login(QString account, QString pass);
public slots:
    void slotLoginButtonClicked(bool);
};

#endif // MYLOGIN_H
