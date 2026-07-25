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

    if (isMyMessage)
    {
        ui->frameBubble->setStyleSheet(
            "QFrame#frameBubble {"
            "   background-color: rgb(55, 138, 221);"
            "   border-radius: 14px;"
            "   padding: 8px 12px;"
            "}"
            "QLabel {"
            "   color: white;"
            "   background: transparent;"
            "}"
        );
        ui->lblText->setAlignment(Qt::AlignRight);
    }
    else
    {
        ui->frameBubble->setStyleSheet(
            "QFrame#frameBubble {"
            "   background-color: rgb(60, 60, 60);"
            "   border-radius: 14px;"
            "   padding: 8px 12px;"
            "}"
            "QLabel {"
            "   color: rgb(220, 220, 220);"
            "   background: transparent;"
            "}"
        );
    }

    int frameIndex = ui->horizontalLayout->indexOf(ui->frameBubble);
    int totalItems = ui->horizontalLayout->count();

    for (int i = 0; i < totalItems; ++i)
    {
        if (i == frameIndex)
            continue;

        QSpacerItem *spacer = ui->horizontalLayout->itemAt(i)->spacerItem();
        if (!spacer)
            continue;

        bool isLeftSpacer = (i < frameIndex);
        bool shouldExpand = isMyMessage ? isLeftSpacer : !isLeftSpacer;

        if (shouldExpand)
            spacer->changeSize(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        else
            spacer->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    }

    ui->horizontalLayout->invalidate();
}

textChat::~textChat()
{
    delete ui;
}