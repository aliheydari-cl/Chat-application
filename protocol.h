#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>

class protocol : public QObject
{
    Q_OBJECT
public:
    explicit protocol(QObject *parent = nullptr);

    QString _name;
    QString _path;
    qint64 _size;
    QByteArray _data;

    enum type{
        isTyping,
        NameChange,
        initSendFile,
        sendRejectionFile,
        sendFile,
        acceptedSendFile,        
    };

    type getType() const;
    void setType(type newType);
    void deleteType();

    QString name() const;

    QString path() const;

    qint64 size() const;

    QByteArray data() const;

public slots:
    QByteArray setStatus();
    QByteArray setName(QString name);
    QByteArray setInitSendFile(QString clientName, qint64 size);
    QByteArray setSendRejectionFile();
    QByteArray setSendFile(QString clientName, qint64 size, QByteArray data);
    QByteArray setAcceptedSendFile();

    void loadData(QByteArray data);

signals:

private:
    type _type;
};

#endif // PROTOCOL_H
