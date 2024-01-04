#include "chatwidget.h"
#include "ui_chatwidget.h"

#include <QFileDialog>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QMessageBox>

ChatWidget::ChatWidget(QTcpSocket *socket, QWidget *parent, bool isServer)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
    , _isServer(isServer)
    , _socket(socket)
{
    ui->setupUi(this);
    if(isServer)
    {
        ui->nameFrame->deleteLater();

        QDir dir;
        QString path = QDir::currentPath() + "/Server";
        dir.mkdir(path);
        _openFilePath = path;
    }
    else
    {
        QString filePath = QDir::currentPath();

        int i = 1;
        QString path = filePath + QString("/Client %1").arg(i);
        QDir dir;

        while (dir.exists(path))
            path = filePath + QString("/Client %1").arg(++i);;

        dir.mkdir(path);
        _openFilePath = path;
    }

    connect(_socket, &QTcpSocket::readyRead, this, &ChatWidget::redyRead);
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::redyRead()
{
    QByteArray const data = _socket->readAll();

    _protocol.loadData(data);

    switch (_protocol.getType()) {
    case protocol::isTyping:
        emit isTyping();
        return;
        break;

    case protocol::NameChange:
        emit nameChanged(_protocol.name());

        return;
        break;

    case protocol::initSendFile:
        emit initSendFile(_protocol._path, _protocol._size);

        return;
        break;

    case protocol::sendFile:
        dataReceived(_protocol._path, _protocol.data());

        return;
        break;
    case protocol::sendRejectionFile:
        fileRejected();
        return;
        break;

    case protocol::acceptedSendFile:
        sendFile();
        return;
        break;

    default:
        break;
    }

    auto _textChat = new textChat();
    _textChat->setMasseage(data);

    QListWidgetItem *listWidgetItem = new QListWidgetItem();
    ui->lwChat->addItem(listWidgetItem);
    listWidgetItem->setSizeHint(QSize(0, 65));
    listWidgetItem->setBackground(QColor(65,60,50,50));
    ui->lwChat->setItemWidget(listWidgetItem, _textChat);
}

void ChatWidget::on_btnSend_clicked()
{

    QString data = ui->leData->text().trimmed();
    if(data == "")
        return;

    auto _textChat = new textChat();
    _textChat->setMasseage(data, true);

    QListWidgetItem *listWidgetItem = new QListWidgetItem();
    ui->lwChat->addItem(listWidgetItem);
    listWidgetItem->setSizeHint(QSize(0, 65));
    listWidgetItem->setBackground(QColor(120,120,120,120));
    ui->lwChat->setItemWidget(listWidgetItem, _textChat);

    if(_socket->isOpen())
        _socket->write(data.toUtf8());
    else
        qDebug() << "Error";

    ui->leData->setText("");
}

void ChatWidget::on_leData_textChanged()
{
    _socket->write(_protocol.setStatus());
}

void ChatWidget::on_leName_editingFinished()
{
    QString name = ui->leName->text().trimmed();
    _socket->write(_protocol.setName(name));
    _socket->setProperty("name", name);
    ui->leName->setText("");
    QFile file(_openFilePath);
    file.close();
    file.rename(name);
    _openFilePath = QFileInfo(file).absoluteFilePath();
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
        qDebug() << "Error!";

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

void ChatWidget::on_lblOpen_linkActivated()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(_openFilePath));
}


