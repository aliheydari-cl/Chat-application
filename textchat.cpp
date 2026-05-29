#include "textchat.h"
#include "ui_textchat.h"

#include <QTime>

textChat::textChat(QWidget *parent, const QString &message, const bool &isMyMessage)
    : QWidget(parent)
    , ui(new Ui::textChat)
{
    ui->setupUi(this);

    ui->lblText->setText(message);
    ui->lblTime->setText(QTime::currentTime().toString("hh:mm"));

    if(isMyMessage)
        ui->lblText->setAlignment(Qt::AlignRight);
}

textChat::~textChat()
{
    delete ui;
}

