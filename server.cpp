#include "server.h"
#include <QTcpServer>
#include <QObject>

Server::Server(QObject *parent, bool isServer)
    : QObject{parent}
{
    _server = nullptr;
    if(isServer == true)
        setUpServer();
    else if(isServer == false)
        setUpClient();
}

void Server::setUpServer()
{
    QTcpServer *server = new QTcpServer();
    server->listen(QHostAddress::Any, 6000);
    connect(server, &QTcpServer::newConnection, this, &Server::newConection);

}

void Server::setUpClient()
{
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost(QHostAddress::LocalHost, 6000);
    connect(socket, &QTcpSocket::connected, this, &Server::clientConnected);

}

void Server::newConection()
{
    auto server = qobject_cast<QTcpServer *>(sender());
    auto client = server->nextPendingConnection();
    socketList << client;
    auto id = socketList.length();
    client->setProperty("id", id);

    connect(client, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

    emit server_newClientConnected(client);

}

void Server::clientDisconnected()
{
    auto socket = qobject_cast<QTcpSocket *>(sender());

    socketList.removeOne(socket);
}

void Server::clientConnected()
{
    auto client = qobject_cast<QTcpSocket *>(sender());

    connect(client, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

    emit server_ConnectedToServer(client);

}


















