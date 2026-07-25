#include "server.h"
#include <QTcpServer>
#include <QObject>
#include <networkutils.h>

Server::Server(QObject *parent)
    : QObject{parent}
{
    server = new QTcpServer(this);

    if (!server->listen(QHostAddress::Any, 6000)) {
        qWarning() << "Server failed to start:" << server->errorString();
    }

    connect(server, &QTcpServer::newConnection, this, &Server::newConection);
}

void Server::sendMessage(QString message, QString receiverName)
{
    QTcpSocket *socket = socketList.value(receiverName, nullptr);

    if (socket != nullptr)
        sendPacket(socket, protocol.setSendMessage(message, receiverName));
}

void Server::newConection()
{
    QTcpSocket* socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

    socket->setProperty("id", ++nextId);

    QString name = QString("Chat %1").arg(socket->property("id").toString());

    socket->setProperty("name", name);

    socketList[name] = socket;

    sendPacket(socket, protocol.setSendInformation(name, socketList.keys()));

    for (QTcpSocket *s : socketList.values()) {
        if(s != socket)
            sendPacket(s, protocol.setSendNewClient(name));
    }

    emit newClientConnected(socket);
}

void Server::clientDisconnected()
{
    auto _socket = qobject_cast<QTcpSocket *>(sender());

    if (!_socket)
        return;

    QString name = _socket->property("name").toString();
    socketList.remove(name);

    for (QTcpSocket *socket : socketList.values())
        sendPacket(socket, protocol.setSendDisconnectClient(name));

    emit sendClientDisconnected(_socket);
    _socket->deleteLater();
}

bool Server::setChangeName(QString prevName, QString newName)
{
    if (socketList.contains(newName))
        return false;

    QTcpSocket *_socket = socketList.value(prevName);

    if(!_socket)
        return false;

    socketList.remove(prevName);
    socketList.insert(newName, _socket);
    _socket->setProperty("name", newName);


    for (QTcpSocket *socket : socketList.values())
    {
        if(socket != _socket)
            sendPacket(socket, protocol.setSendNameChange(prevName, newName));
    }

    return true;
}

void Server::sendNameChangeRejected(QTcpSocket *socket)
{
    if (socket)
        sendPacket(socket, protocol.setSendNameChangeRejected());
}







