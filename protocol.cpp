#include "protocol.h"
#include "qdebug.h"

#include <QIODevice>

protocol::protocol(QObject *parent)
    : QObject{parent}
{}

QByteArray protocol::setStatus()
{
    QByteArray ba;

    QDataStream out(&ba, QIODevice::WriteOnly);

    out << isTyping;

    return ba;
}

QByteArray protocol::setName(QString name)
{
    this->name = name;

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);

    out << NameChange << name;

    return ba;

}

void protocol::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in >> _type;

    if(_type == NameChange)
        in >> name;

}

protocol::type protocol::getType() const
{
    return _type;
}

void protocol::setType(type newType)
{
    _type = newType;
}

void protocol::deleteType()
{
    //_type = none;
}


