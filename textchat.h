#ifndef TEXTCHAT_H
#define TEXTCHAT_H

#include <QWidget>

namespace Ui {
class textChat;
}

class textChat : public QWidget
{
    Q_OBJECT

public:
    explicit textChat(QWidget *parent = nullptr);
    void setMasseage(QString message, bool isMyMessage = false);
    ~textChat();


private:
    Ui::textChat *ui;
};

#endif // TEXTCHAT_H
