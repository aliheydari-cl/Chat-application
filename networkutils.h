#pragma once

#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>

inline void sendPacket(QTcpSocket *socket, const QByteArray &payload)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out << payload;
    socket->write(packet);
}