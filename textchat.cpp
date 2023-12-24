#include "textchat.h"
#include "ui_textchat.h"

#include <QTime>

textChat::textChat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::textChat)
{
    ui->setupUi(this);
}

textChat::~textChat()
{
    delete ui;
}

void textChat::setMasseage(QString message, bool isMyMessage)
{
    ui->lblText->setText(message);
    ui->lblTime->setText(QTime::currentTime().toString("hh:mm"));
    if(isMyMessage)
        ui->lblText->setAlignment(Qt::AlignRight);
}
