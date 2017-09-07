#ifndef MYLISTWIDGETITEM_H
#define MYLISTWIDGETITEM_H
#include <QListWidgetItem>

class MyListWidgetItem : public QListWidgetItem
{
public:
    MyListWidgetItem(const char* buf, int len);
    ~MyListWidgetItem();

    void SetBuf(const char* buf, int len);
    char* GetBuf(int& len);
private:
    char* m_buf;
    int m_len;
};


#endif // MYLISTWIDGETITEM_H
