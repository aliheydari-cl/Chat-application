#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QTcpSocket>

class Protocol : public QObject
{
    Q_OBJECT
public:
    explicit Protocol(QObject *parent = nullptr);

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
        sendNameChangeRejected
    };

    type getType() const;

    void setType(type newType);
    void deleteType();

    QString getName() const;
    QString getPath() const;
    QString getMessage() const;
    QString getPrevName() const;
    QString getNewName() const;
    QStringList getList() const;
    QString getReceiverName() const;
    QString getSenderName() const;

    qint64 getSize() const;

    QByteArray getData() const;

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
    QByteArray setSendNameChangeRejected();


    void loadData(QByteArray rawData);

signals:

private:
    QString name;
    QString _message;
    QString prevName;
    QString newName;
    QStringList list;
    QString receiverName;
    QString senderName;
    QString path;

    type _type;

    qint64 size;
    QByteArray data;
};

#endif // PROTOCOL_H
