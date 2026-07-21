#include "chatwidget.h"
#include "ui_chatwidget.h"

#include <QFileDialog>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QMessageBox>
#include <networkutils.h>

ChatWidget::ChatWidget(QWidget *parent, QTcpSocket *socket, bool isServer)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
    , isServer(isServer)
    , socket(socket)
{
    ui->setupUi(this);

    if(isServer)
    {
        ui->nameFrame->deleteLater();
        ui->cbClients->deleteLater();

        QDir dir;
        QString path = QDir::currentPath() + "/Server";
        dir.mkdir(path);
        openFilePath = path;

        myName = "Server";
    }

    connect(socket, &QTcpSocket::readyRead, this, &ChatWidget::redyRead);
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::setInformation(QString name, QStringList list)
{
    QString filePath = QDir::currentPath();
    QString path = filePath + "/" + name;
    QDir dir;

    dir.mkdir(path);

    openFilePath = path;
    myName = name;
    socket->setProperty("name", name);

    ui->cbClients->addItem("Server");

    foreach (QString var, list) {
        if(var != myName)
            ui->cbClients->addItem(var);
    }
}

void ChatWidget::redyRead()
{
    buffer.append(socket->readAll());

    while(true)
    {
        QDataStream in(buffer);
        in.startTransaction();

        QByteArray oneMessage;
        in >> oneMessage;

        if(!in.commitTransaction())
            return;

        buffer.remove(0,in.device()->pos());


        protocol.loadData(oneMessage);

        switch (protocol.getType()) {
        case Protocol::sendInformation:
        {
            setInformation(protocol.getName(), protocol.getList());
            socket->setProperty("name", protocol.getName());
            break;
        }

        case Protocol::message:
            loadMessage(protocol.getMessage(), protocol.getReceiverName());
            break;

        case Protocol::isTyping:
            emit isTyping();
            break;

        case Protocol::nameChange:
        {
            emit nameChanged(protocol.getPrevName(), protocol.getNewName());
            if(isServer)
                socket->setProperty("name", protocol.getNewName());
            break;
        }

        case Protocol::initSendFile:
            emit initSendFile(protocol.getPath(), protocol.getSize());

            break;

        case Protocol::sendFile:
            dataReceived(protocol.getPath(), protocol.getData());

            break;

        case Protocol::sendRejectionFile:
            fileRejected();

            break;

        case Protocol::acceptedSendFile:
            sendFile();

            break;

        case Protocol::sendNewClient:
            newClientReceived(protocol.getName());

            break;

        case Protocol::sendDisconnectClient:
            clientDisconnected(protocol.getName());

            break;

        case Protocol::sendNameChangeClient:

            clientNameChange(protocol.getPrevName(), protocol.getNewName());

            break;

        case Protocol::sendNameChangeRejected:

            clientNameChangeRejected();

            break;

        default:
            break;

        }
    }
}

void ChatWidget::on_btnSend_clicked()
{
    QString receiverName;
    QString message = ui->leData->text().trimmed();

    if(message == "")
        return;

    if(isServer)
        receiverName = socket->property("name").toString();
    else
        receiverName = ui->cbClients->currentText();

    if(!socket->isOpen())
        qDebug() << "Error";

    sendPacket(socket, protocol.setSendMessage(message, receiverName));

    setMessage(message, true);

    ui->leData->setText("");
}

void ChatWidget::on_leData_textChanged()
{
    sendPacket(socket, protocol.setStatus());
}

void ChatWidget::on_leName_editingFinished()
{
    QString name = ui->leName->text().trimmed();

    if(name == "")
        return;

    sendPacket(socket, protocol.setName(myName ,name));

    ui->leName->setText("");

    QFile file(openFilePath);
    file.rename(name);
    file.close();

    openFilePath = QFileInfo(file).absoluteFilePath();
    myName = name;
    socket->setProperty("name", name);
}

void ChatWidget::on_leData_editingFinished()
{
    on_btnSend_clicked();
}

void ChatWidget::on_btnSendFile_clicked()
{
    QString nameFile = QFileDialog::getOpenFileName(this, "Inter File", "/home");
    if(nameFile.length() < 1)
        return;
    QFileInfo info(nameFile);
    size = info.size();

    sendFilePath = nameFile;

    QFile file(sendFilePath);

    if(!file.open(QIODevice::ReadOnly))
        qDebug() << "Error";

    data = file.readAll();
    file.close();

    sendPacket(socket, protocol.setInitSendFile(sendFilePath, size));
}

void ChatWidget::dataReceived(QString path, QByteArray data)
{
    QFileInfo info(path);

    QString name = info.fileName();

    QFile file(openFilePath + QString("/%1").arg(name));

    if(file.open(QIODevice::WriteOnly))
        file.write(data);
    else
        qDebug() << "Error";

    file.close();
}

void ChatWidget::sendFile()
{
    sendPacket(socket, protocol.setSendFile(sendFilePath, size, data));
}

void ChatWidget::acceptedSendFile()
{
    sendPacket(socket, protocol.setAcceptedSendFile());

}

void ChatWidget::setFileRejected()
{
    sendPacket(socket, protocol.setSendRejectionFile());
}

void ChatWidget::fileRejected()
{
    QMessageBox::warning(this, "File rejected", "The initiation for sending the file has been rejected!");
}

void ChatWidget::clientDisconnected(QString name)
{
    int index = ui->cbClients->findText(name);

    if (index != -1)
        ui->cbClients->removeItem(index);
}

void ChatWidget::on_lblOpen_linkActivated()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(openFilePath));
}

void ChatWidget::newClientReceived(QString name)
{
    if(name != myName)
        ui->cbClients->addItem(name);
}

void ChatWidget::clientNameChange(QString prevName, QString newName)
{
    int index = ui->cbClients->findText(prevName);

    if (index != -1)
        ui->cbClients->setItemText(index, newName);
}

void ChatWidget::clientNameChangeRejected()
{
    QMessageBox::warning(this, "Rename failed", "This name is already taken.");
}

void ChatWidget::setMessage(QString message, bool isMyMessage)
{
    auto _textChat = new textChat(this, message, isMyMessage);

    QListWidgetItem *listWidgetItem = new QListWidgetItem();
    ui->lwChat->addItem(listWidgetItem);
    listWidgetItem->setSizeHint(QSize(0, 65));
    QColor backgroundColor = isMyMessage ? QColor("#A3806D") : QColor("#706761");
    listWidgetItem->setBackground(backgroundColor);

    ui->lwChat->setItemWidget(listWidgetItem, _textChat);
}

void ChatWidget::loadMessage(QString message, QString receiverName)
{
    if(isServer)
        receiverName == "Server" ? setMessage(message, false) : emit sendMessage(message, receiverName);
    else if(receiverName == socket->property("name").toString())
        setMessage(message, false);
}






