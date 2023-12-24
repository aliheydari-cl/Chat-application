#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include "server.h"
#include <textchat.h>
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QTcpSocket *socket, QWidget *parent = nullptr);
    ~ChatWidget();

signals:
    void chatWidget_datarecived(QByteArray data);

private slots:

    void redyRead();

    void on_btnSend_clicked();

private:
    Ui::ChatWidget *ui;

    QTcpSocket *_socket;
    QTcpServer *_server;
};

#endif // CHATWIDGET_H
