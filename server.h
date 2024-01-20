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
    explicit Server(QObject *parent = nullptr, bool isServer = false);
    void setUpServer();
    void setUpClient();
    void setChangeName(QString prevName, QString newName);

public slots:
    void sendMessage(QString message, QString receiverName);

signals:
    void newClientConnected(QTcpSocket *client);
    void connectedToServer(QTcpSocket *client);
    void sendClientDisconnected(QTcpSocket *);

private slots:
    void newConection();
    void clientDisconnected();
    void clientConnectedToServer();

private:
    QMap<QString, QTcpSocket *> _socketList;
    bool _isServer = false;

    QTcpServer *_server;
    ushort _port;
    textChat *_textChat;
    protocol _protocol;
    QTcpSocket *_socket;
};

#endif // SERVER_H
