#include "protocol.h"
#include "qdebug.h"

#include <QCoreApplication>
#include <QDir>
#include <QIODevice>

Protocol::Protocol(QObject *parent)
    : QObject{parent}
{}

QByteArray Protocol::setStatus()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << isTyping;

    return ba;
}

QByteArray Protocol::setName(QString prevName, QString newName)
{
    this->name = newName;

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);

    out << nameChange << prevName << newName;

    return ba;
}

QByteArray Protocol::setInitSendFile(QString clientName, qint64 size)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << initSendFile << clientName << size;

    return ba;
}

QByteArray Protocol::setSendRejectionFile()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendRejectionFile;

    return ba;
}

QByteArray Protocol::setSendFile(QString path, qint64 size, QByteArray data)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendFile << path << size << data;

    return ba;
}

QByteArray Protocol::setAcceptedSendFile()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << acceptedSendFile;

    return ba;

}

QByteArray Protocol::setSendNewClient(QString name)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendNewClient << name;

    return ba;

}

QByteArray Protocol::setSendDisconnectClient(QString name)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendDisconnectClient << name;

    return ba;

}

QByteArray Protocol::setSendNameChange(QString prevName, QString newName)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendNameChangeClient << prevName << newName;

    return ba;

}

QByteArray Protocol::setSendNameChangeRejected()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendNameChangeRejected;

    return ba;

}
QByteArray Protocol::setSendMessage(QString msg, QString receiverName)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << message << msg << receiverName;

    return ba;
}

void Protocol::loadData(QByteArray rawData)
{
    QDataStream in(&rawData, QIODevice::ReadOnly);
    in >> _type;

    switch (_type) {
    case sendInformation:
        in >> name >> list;

        break;

    case message:
        in >> _message >> receiverName >> senderName;

        break;

    case nameChange:
        in >> prevName >> newName;

        break;

    case initSendFile:
        in >> path;
        in >> size;

        break;

    case sendFile:
        in >> path;
        in >> size;
        in >> data;

        break;

    case sendRejectionFile:

        break;

    case acceptedSendFile:

        break;

    case sendNewClient:
        in >> name;

        break;

    case sendDisconnectClient:
        in >> name;

        break;

    case sendNameChangeClient:
        in >> prevName >> newName;

        break;

    case sendNameChangeRejected:

        break;


    default:

        break;
    }
}

QByteArray Protocol::setSendInformation(QString name, QStringList list)
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << sendInformation << name << list;

    return ba;
}


QByteArray Protocol::getData() const
{
    return data;
}

qint64 Protocol::getSize() const
{
    return size;
}

QString Protocol::getPath() const
{
    return path;
}

QString Protocol::getName() const
{
    return name;
}

QString Protocol::getSenderName() const
{
    return senderName;
}

QString Protocol::getReceiverName() const
{
    return receiverName;
}

QStringList Protocol::getList() const
{
    return list;
}

QString Protocol::getNewName() const
{
    return newName;
}

QString Protocol::getPrevName() const
{
    return prevName;
}

QString Protocol::getMessage() const
{
    return _message;
}


Protocol::type Protocol::getType() const
{
    return _type;
}

void Protocol::setType(type newType)
{
    _type = newType;
}


