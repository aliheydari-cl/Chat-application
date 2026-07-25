#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QObject *parent = nullptr);
    ~ClientManager();

signals:
    void clientConnectedToServer(QTcpSocket*);
private:
    QTcpSocket* socket;

    void ClientConnectedToServer();
};

#endif // CLIENTMANAGER_H
