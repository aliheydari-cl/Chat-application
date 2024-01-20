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
    explicit ChatWidget(QWidget *parent = nullptr, QTcpSocket *socket = nullptr, bool isServer = false);
    ~ChatWidget();

public slots:
    void sendFile();
    void acceptedSendFile();
    void setFileRejected();
    void setInformation(QString name, QStringList list);
    void newClientReceived(QString name);
    void clientNameChange(QString prevName, QString newName);
    void loadMessage(QString message, QString receiverName);
    void setMessage(QString message, bool isMyMessage = false);


signals:
    void isTyping();
    void nameChanged(QString, QString);
    void initSendFile(QString, qint64);
    void sendFile(QString, qint64, QByteArray);
    void btnSendClicked(QString, qint64, bool);
    void sendMessage(QString message, QString receiverName);

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
    void clientDisconnected(QString name);

private:  
    Ui::ChatWidget *ui;
    QString _myName;
    bool _isServer;
    QTcpSocket *_socket;
    protocol _protocol;
    QString _sendFilePath;
    QString _openFilePath;
    qint64 _size;
    QByteArray _data;
};

#endif // CHATWIDGET_H
