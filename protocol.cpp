#include "protocol.h"
#include "qdebug.h"

#include <QCoreApplication>
#include <QDir>
#include <QIODevice>

protocol::protocol(QObject *parent)
    : QObject{parent}
{

}

QByteArray protocol::setStatus()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << isTyping;

    return ba;
}

QByteArray protocol::setName(QString prevName, QString newName)
{
    this->_name = newName;

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);

    out << nameChange << prevName << newName;

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

QByteArray protocol::setSendNewClient(QString name)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendNewClient << name;

    return ba;

}

QByteArray protocol::setSendDisconnectClient(QString name)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendDisconnectClient << name;

    return ba;

}

QByteArray protocol::setSendNameChange(QString prevName, QString newName)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendNameChangeClient << prevName << newName;

    return ba;

}
QByteArray protocol::setSendMessage(QString msg, QString receiverName)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << message << msg << receiverName;

    return ba;
}

void protocol::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in >> _type;

    switch (_type) {
    case sendInformation:
        in >> _name >> _list;

        break;

    case message:
        in >> _message >> _receiverName >> _senderName;

        break;

    case nameChange:
        in >> _prevName >> _newName;

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

    case sendNewClient:
        in >> _name;

        break;

    case sendDisconnectClient:
        in >> _name;

        break;

    case sendNameChangeClient:
        in >> _prevName >> _newName;

        break;

    default:

        break;
    }
}

QByteArray protocol::setSendInformation(QString name, QStringList list)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendInformation << name << list;

    return ba;
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

QString protocol::senderName() const
{
    return _senderName;
}

QString protocol::receiverName() const
{
    return _receiverName;
}

QStringList protocol::list() const
{
    return _list;
}

QString protocol::newName() const
{
    return _newName;
}

QString protocol::prevName() const
{
    return _prevName;
}

QString protocol::getMessage() const
{
    return _message;
}


protocol::type protocol::getType() const
{
    return _type;
}

void protocol::setType(type newType)
{
    _type = newType;
}


