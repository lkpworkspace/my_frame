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

    char* GetBuf(int& len);
    int GetLen();
public slots:
    void OnButClicked();
signals:
    void DelMySelf();

private:
    void AddComboBoxItem(const QString& str);
    QPushButton* m_butDel;
    QComboBox* m_comBox;
    QLineEdit* m_edit;
    int index;
};

#endif // MYBUFITEM_H
