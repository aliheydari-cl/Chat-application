#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <textchat.h>
#include <protocol.h>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool setChangeName(QString prevName, QString newName);
    void sendNameChangeRejected(QTcpSocket *socket);

public slots:
    void sendMessage(QString message, QString receiverName);

signals:
    void newClientConnected(QTcpSocket *client);
    void sendClientDisconnected(QTcpSocket *);

private slots:
    void newConection();
    void clientDisconnected();

private:
    QMap<QString, QTcpSocket *> socketList;
    bool isServer = false;

    QTcpServer *server;
    ushort port;
    textChat *textChat;
    Protocol protocol;
    //QTcpSocket *socket;
    int nextId = 0;
};

#endif // SERVER_H
