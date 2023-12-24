#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <textchat.h>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr, bool isServer = false);
    void setUpServer();
    void setUpClient();

signals:
    void server_newClientConnected(QTcpSocket *client);
    void server_ConnectedToServer(QTcpSocket *client);

private slots:
    void newConection();

    void clientDisconnected();

    void clientConnected();


private:
    QTcpServer *_server;
    QList<QTcpSocket *> socketList;

    ushort _port;
    textChat *_textChat;
};

#endif // SERVER_H
