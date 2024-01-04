#ifndef CHATWIDGET_H
#define CHATWIDGET_H

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
    QString _name;

public slots:
    void sendFile();
    void acceptedSendFile();
    void setFileRejected();


signals:
    void isTyping();

    void nameChanged(QString);
    void initSendFile(QString, qint64);
    void sendFile(QString, qint64, QByteArray);
    void btnSendClicked(QString, qint64, bool);

private slots:

    void redyRead();

    void on_btnSend_clicked();

    void on_leData_textChanged();

    void on_leName_editingFinished();

    void on_leData_editingFinished();

    void on_btnSendFile_clicked();

    void dataReceived(QString, QByteArray);

    void on_lblOpen_linkActivated();

    void fileRejected();

private:
    Ui::ChatWidget *ui;

    bool _isServer;

    QTcpSocket *_socket;

    protocol _protocol;

    QString _sendFilePath;
    QString _openFilePath;

    qint64 _size;
    QByteArray _data;
};

#endif // CHATWIDGET_H
