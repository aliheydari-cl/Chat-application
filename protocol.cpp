#include "protocol.h"
#include "qdebug.h"

#include <QCoreApplication>
#include <QDir>
#include <QIODevice>

protocol::protocol(QObject *parent)
    : QObject{parent}
{

}

QByteArray protocol::data() const
{
    return _data;
}

qint64 protocol::size() const
{
    return _size;
}

QString protocol::path() const
{
    return _path;
}

QString protocol::name() const
{
    return _name;
}

QByteArray protocol::setStatus()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << isTyping;

    return ba;
}

QByteArray protocol::setName(QString name)
{
    this->_name = name;

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);

    out << NameChange << name;

    return ba;
}

QByteArray protocol::setInitSendFile(QString clientName, qint64 size)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << initSendFile << clientName << size;

    return ba;
}

QByteArray protocol::setSendRejectionFile()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendRejectionFile;

    return ba;
}

QByteArray protocol::setSendFile(QString path, qint64 size, QByteArray data)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendFile << path << size << data;

    return ba;
}

QByteArray protocol::setAcceptedSendFile()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << acceptedSendFile;

    return ba;

}

void protocol::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in >> _type;

    switch (_type) {
    case NameChange:
        in >> _name;
        break;

    case initSendFile:
        in >> _path;
        in >> _size;
        break;

    case sendFile:
        in >> _path;
        in >> _size;
        in >> _data;
        break;

    case sendRejectionFile:
        break;

    case acceptedSendFile:
        break;

    default:
        break;
    }

}

protocol::type protocol::getType() const
{
    return _type;
}

void protocol::setType(type newType)
{
    _type = newType;
}


