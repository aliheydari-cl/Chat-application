#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include "server.h"
#include <textchat.h>
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <protocol.h>

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QTcpSocket *socket, QWidget *parent = nullptr, bool isServer = false);
    ~ChatWidget();

signals:
    void chatWidget_datarecived(QByteArray data);

    void isTyping();

    void nameChanged(QString);


private slots:

    void redyRead();

    void on_btnSend_clicked();

    void on_leData_textChanged();

    void on_leName_editingFinished();

    void on_leData_editingFinished();

private:
    Ui::ChatWidget *ui;

    QTcpSocket *_socket;
    QTcpServer *_server;

    protocol _protocol;
};

#endif // CHATWIDGET_H
