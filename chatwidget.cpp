#include "chatwidget.h"
#include "ui_chatwidget.h"

#include <QListWidgetItem>

ChatWidget::ChatWidget(QTcpSocket *socket, QWidget *parent, bool isServer)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
    , _socket(socket)
{
    ui->setupUi(this);
    if(isServer)
        ui->nameFrame->deleteLater();

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
        emit nameChanged(_protocol.name);

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
        qDebug() << "socket Error";
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
}


void ChatWidget::on_leData_editingFinished()
{
    on_btnSend_clicked();
}

