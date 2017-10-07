#ifndef MYBUFITEM_H
#define MYBUFITEM_H
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QByteArray>

class MyBufItem : public QWidget
{
    Q_OBJECT
public:
    MyBufItem(QWidget* parent = 0);
    ~MyBufItem();

    char* GetBuf(int& len);
    int GetLen();
public slots:
    void OnButClicked();
signals:
    void DelMySelf();

private:
    char* ToData(QByteArray, int& outlen);
    void AddComboBoxItem(const QString& str);
    QPushButton* m_butDel;
    QComboBox* m_comBox;
    QLineEdit* m_edit;
    int index;
    char* m_buf;
};

#endif // MYBUFITEM_H
