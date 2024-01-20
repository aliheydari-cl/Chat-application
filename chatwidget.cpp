#include "chatwidget.h"
#include "ui_chatwidget.h"

#include <QFileDialog>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QMessageBox>

ChatWidget::ChatWidget(QWidget *parent, QTcpSocket *socket, bool isServer)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
    , _isServer(isServer)
    , _socket(socket)
{
    ui->setupUi(this);
    if(isServer)
    {
        ui->nameFrame->deleteLater();
        ui->cbClients->deleteLater();

        QDir dir;
        QString path = QDir::currentPath() + "/Server";
        dir.mkdir(path);
        _openFilePath = path;

        _myName = "Server";
    }

    connect(_socket, &QTcpSocket::readyRead, this, &ChatWidget::redyRead);
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

    _openFilePath = path;
    _myName = name;
    _socket->setProperty("name", name);

    ui->cbClients->addItem("Server");

    foreach (QString var, list) {
        if(var != _myName)
            ui->cbClients->addItem(var);
    }
}


void ChatWidget::redyRead()
{
    QByteArray const data = _socket->readAll();

    _protocol.loadData(data);

    switch (_protocol.getType()) {
    case protocol::sendInformation:
    {
        setInformation(_protocol.name(), _protocol.list());
        _socket->setProperty("name", _protocol.name());
        break;
    }

    case protocol::message:
        loadMessage(_protocol.getMessage(), _protocol.receiverName());
        break;

    case protocol::isTyping:
        emit isTyping();
        break;

    case protocol::nameChange:
    {
        emit nameChanged(_protocol.prevName(), _protocol.newName());
        if(_isServer)
            _socket->setProperty("name", _protocol.newName());
        break;
    }

    case protocol::initSendFile:
        emit initSendFile(_protocol.path(), _protocol.size());

        break;

    case protocol::sendFile:
        dataReceived(_protocol.path(), _protocol.data());

        break;

    case protocol::sendRejectionFile:
        fileRejected();

        break;

    case protocol::acceptedSendFile:
        sendFile();

        break;

    case protocol::sendNewClient:
        newClientReceived(_protocol.name());

        break;

    case protocol::sendDisconnectClient:
        clientDisconnected(_protocol.name());

        break;

    case protocol::sendNameChangeClient:

        clientNameChange(_protocol.prevName(), _protocol.newName());

        break;

    default:
        break;

    }
}

void ChatWidget::on_btnSend_clicked()
{
    QString receiverName;
    QString message = ui->leData->text().trimmed();

    if(message == "")
        return;

    if(_isServer)
        receiverName = _socket->property("name").toString();
    else
        receiverName = ui->cbClients->currentText();

    if(!_socket->isOpen())
        qDebug() << "Error";
    _socket->write(_protocol.setSendMessage(message, receiverName));

    setMessage(message, true);

    ui->leData->setText("");
}

void ChatWidget::on_leData_textChanged()
{
    _socket->write(_protocol.setStatus());
}

void ChatWidget::on_leName_editingFinished()
{
    QString name = ui->leName->text().trimmed();

    if(name == "")
        return;

    _socket->write(_protocol.setName(_myName ,name));

    ui->leName->setText("");

    QFile file(_openFilePath);
    file.rename(name);
    file.close();

    _openFilePath = QFileInfo(file).absoluteFilePath();
    _myName = name;
    _socket->setProperty("name", name);
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
    qint64 size = info.size();
    _size = size;

    _sendFilePath = nameFile;

    QByteArray data;

    QFile file(_sendFilePath);

    if(!file.open(QIODevice::ReadOnly))
        qDebug() << "Error";

    data = file.readAll();
    _data = data;
    file.close();

    _socket->write(_protocol.setInitSendFile(_sendFilePath, _size));
}

void ChatWidget::dataReceived(QString path, QByteArray data)
{
    QFileInfo info(path);

    QString name = info.fileName();

    QFile file(_openFilePath + QString("/%1").arg(name));

    if(file.open(QIODevice::WriteOnly))
        file.write(data);
    else
        qDebug() << "Error";

    file.close();
}

void ChatWidget::sendFile()
{
    _socket->write(_protocol.setSendFile(_sendFilePath, _size, _data));
}

void ChatWidget::acceptedSendFile()
{
    _socket->write(_protocol.setAcceptedSendFile());
}

void ChatWidget::setFileRejected()
{
    _socket->write(_protocol.setSendRejectionFile());
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
    QDesktopServices::openUrl(QUrl::fromLocalFile(_openFilePath));
}

void ChatWidget::newClientReceived(QString name)
{
    if(name != _myName)
        ui->cbClients->addItem(name);
}

void ChatWidget::clientNameChange(QString prevName, QString newName)
{
    int index = ui->cbClients->findText(prevName);

    if (index != -1)
        ui->cbClients->setItemText(index, newName);
}

void ChatWidget::setMessage(QString message, bool isMyMessage)
{
    auto _textChat = new textChat();
    _textChat->setMasseage(message, isMyMessage);

    QListWidgetItem *listWidgetItem = new QListWidgetItem();
    ui->lwChat->addItem(listWidgetItem);
    listWidgetItem->setSizeHint(QSize(0, 65));
    QColor backgroundColor = isMyMessage ? QColor("#A3806D") : QColor("#706761");
    listWidgetItem->setBackground(backgroundColor);

    ui->lwChat->setItemWidget(listWidgetItem, _textChat);
}

void ChatWidget::loadMessage(QString message, QString receiverName)
{
    if(_isServer)          
        receiverName == "Server" ? setMessage(message, false) : emit sendMessage(message, receiverName);
    else if(receiverName == _socket->property("name").toString())
            setMessage(message, false);
}






