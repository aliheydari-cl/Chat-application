#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QTcpSocket>

class protocol : public QObject
{
    Q_OBJECT
public:
    explicit protocol(QObject *parent = nullptr);

    enum type{
        sendInformation,
        message,
        isTyping,
        nameChange,
        initSendFile,
        sendRejectionFile,
        sendFile,
        acceptedSendFile,
        sendNewClient,
        sendDisconnectClient,
        sendNameChangeClient,
    };

    type getType() const;
    void setType(type newType);
    void deleteType();

    QString name() const;
    QString path() const;
    QString getMessage() const;
    QString prevName() const;
    QString newName() const;
    QStringList list() const;
    QString receiverName() const;
    QString senderName() const;
    qint64 size() const;
    QByteArray data() const;

public slots:
    QByteArray setSendInformation(QString name, QStringList list) ;
    QByteArray setSendMessage(QString msg, QString receiverName);
    QByteArray setStatus();
    QByteArray setName(QString prevName, QString newName);
    QByteArray setInitSendFile(QString clientName, qint64 size);
    QByteArray setSendRejectionFile();
    QByteArray setSendFile(QString clientName, qint64 size, QByteArray data);
    QByteArray setAcceptedSendFile();
    QByteArray setSendNewClient(QString name);
    QByteArray setSendDisconnectClient(QString name);
    QByteArray setSendNameChange(QString prevName, QString newName);

    void loadData(QByteArray data);

signals:

private:
    type _type;

    QString _name;
    QString _message;
    QString _prevName;
    QString _newName;
    QStringList _list;
    QString _receiverName;
    QString _senderName;
    QString _path;
    qint64 _size;
    QByteArray _data;
};

#endif // PROTOCOL_H
