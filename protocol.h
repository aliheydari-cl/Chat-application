#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>

class protocol : public QObject
{
    Q_OBJECT
public:
    explicit protocol(QObject *parent = nullptr);
    QString name;

    enum type{
        isTyping,
        NameChange,
        none
    };


    type getType() const;
    void setType(type newType);
    void deleteType();

public slots:
    QByteArray setStatus();
    QByteArray setName(QString name);


    void loadData(QByteArray data);

signals:

private:
    type _type;
};

#endif // PROTOCOL_H
