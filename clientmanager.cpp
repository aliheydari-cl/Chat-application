#include "clientmanager.h"

ClientManager::ClientManager(QObject *parent)
    : QObject{parent}
{

    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost, 6000);
    connect(socket, &QTcpSocket::connected, this, &ClientManager::ClientConnectedToServer);

}

ClientManager::~ClientManager()
{
}

void ClientManager::ClientConnectedToServer()
{
    emit clientConnectedToServer(socket);
}
