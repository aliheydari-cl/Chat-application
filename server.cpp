#include "server.h"
#include <QTcpServer>
#include <QObject>

Server::Server(QObject *parent, bool isServer)
    : QObject{parent},
    _isServer(isServer)
{
    _server = nullptr;

    if(_isServer)
        setUpServer();
    else if(!_isServer)
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

    connect(socket, &QTcpSocket::connected, this, &Server::clientConnectedToServer);
}

void Server::sendMessage(QString message, QString receiverName)
{
    QTcpSocket *socket = _socketList.value(receiverName, nullptr);

    if (socket != nullptr)
        socket->write(_protocol.setSendMessage(message, receiverName));
}

void Server::newConection()
{
    auto server = qobject_cast<QTcpServer *>(sender());
    auto client = server->nextPendingConnection();
    connect(client, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

    _socket = client;

    auto id = _socketList.count();

    client->setProperty("id", ++id);

    QString name = QString("Chat %1").arg(client->property("id").toString());

    _socket->setProperty("name", name);

    _socketList[name] = client;

    client->write(_protocol.setSendInformation(name, _socketList.keys()));

    foreach (QTcpSocket *socket, _socketList.values()) {
        if(socket != client)
            socket->write(_protocol.setSendNewClient(client->property("name").toString()));
    }

    emit newClientConnected(client);
}

void Server::clientDisconnected()
{
    auto _socket = qobject_cast<QTcpSocket *>(sender());

    _socketList.remove(_socket->property("name").toString());

    foreach (QTcpSocket *socket, _socketList.values())
    {
        if(socket != _socket)
            socket->write(_protocol.setSendDisconnectClient(_socket->property("name").toString()));
    }
    emit sendClientDisconnected(_socket);
}

void Server::clientConnectedToServer()
{
    auto client = qobject_cast<QTcpSocket *>(sender());

    emit connectedToServer(client);
}

void Server::setChangeName(QString prevName, QString newName)
{
    QTcpSocket *_socket = _socketList.value(prevName, nullptr);
    if (_socket) {
        QStringList keys = _socketList.keys();
        int index = keys.indexOf(prevName);

        if (index != -1)
            keys.replace(index, newName);       

        _socketList.remove(prevName);
        _socketList.insert(newName, _socket);
    }

    foreach (QTcpSocket *socket, _socketList.values())
    {
        if(socket != _socket)
            socket->write(_protocol.setSendNameChange(prevName, newName));
    }
}

















