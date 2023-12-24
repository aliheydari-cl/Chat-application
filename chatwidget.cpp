#include "chatwidget.h"
#include "ui_chatwidget.h"

#include <QListWidgetItem>

ChatWidget::ChatWidget(QTcpSocket *socket, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
    , _socket(socket)
{
    ui->setupUi(this);

    connect(_socket, &QTcpSocket::readyRead, this, &ChatWidget::redyRead);

}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::redyRead()
{
    auto data = _socket->readAll();
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
    auto _textChat = new textChat();

    auto data = ui->leData->text().trimmed();
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


