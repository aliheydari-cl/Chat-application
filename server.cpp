#include "server.h"
#include <QTcpServer>
#include <QObject>

Server::Server(QObject *parent, bool isServer)
    : QObject{parent},
    isServer(isServer)
{
    server = nullptr;

    if(isServer)
        setUpServer();
    else if(!isServer)
        setUpClient();
}

void Server::setUpServer()
{
    server = new QTcpServer();
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
    QTcpSocket *socket = socketList.value(receiverName, nullptr);

    if (socket != nullptr)
        socket->write(protocol.setSendMessage(message, receiverName));
}

void Server::newConection()
{
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

    auto id = socketList.count();

    socket->setProperty("id", ++id);

    QString name = QString("Chat %1").arg(socket->property("id").toString());

    socket->setProperty("name", name);

    socketList[name] = socket;

    socket->write(protocol.setSendInformation(name, socketList.keys()));

    foreach (QTcpSocket *s, socketList.values()) {
        if(s != socket)
            socket->write(protocol.setSendNewClient(socket->property("name").toString()));
    }

    emit newClientConnected(socket);
}

void Server::clientDisconnected()
{
    auto _socket = qobject_cast<QTcpSocket *>(sender());

    socketList.remove(_socket->property("name").toString());

    foreach (QTcpSocket *socket, socketList.values())
    {
        if(socket != _socket)
            socket->write(protocol.setSendDisconnectClient(_socket->property("name").toString()));
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
    QTcpSocket *_socket = socketList.value(prevName, nullptr);
    if (_socket) {
        QStringList keys = socketList.keys();
        int index = keys.indexOf(prevName);

        if (index != -1)
            keys.replace(index, newName);       

        socketList.remove(prevName);
        socketList.insert(newName, _socket);
    }

    foreach (QTcpSocket *socket, socketList.values())
    {
        if(socket != _socket)
            socket->write(protocol.setSendNameChange(prevName, newName));
    }
}






