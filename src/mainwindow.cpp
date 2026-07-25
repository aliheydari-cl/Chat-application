#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <settingdialog.h>
#include <qlineedit.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onInitSendFile(QString path, qint64 size)
{
    auto widget = qobject_cast<ChatWidget *>(sender());
    if (!widget)
        return;

    QString text = QString("name file: %1\n size: %2").arg(path).arg(size);

    auto result = QMessageBox::question(this, "Init send file", text);

    if(result == QMessageBox::Yes)
        widget->acceptedSendFile();
    else
        widget->setFileRejected();
}

void MainWindow::onRejectionSendFile()
{
    QMessageBox::information(this, "Rejection send file", "init file has been rejected!");
}

void MainWindow::newClientConnected(QTcpSocket *socket) //server
{ 
    dialog->serverConnected();
    ChatWidget *chatWidget = new ChatWidget(this, socket, true);

    chatList[socket] = chatWidget;

    ui->twChat->addTab(chatWidget, socket->property("name").toString());

    connect(chatWidget, &ChatWidget::isTyping, this, [this, socket](){

        QString name = socket->property("name").toString();
        if(name.length() < 1)
            name = QString("Client %1").arg(socket->property("id").toInt());

        this->statusBar()->showMessage(QString("%1 is typing...").arg(name), 700);
    });

    connect(chatWidget, &ChatWidget::nameChanged, this, [this](QString prevName, QString newName){

        if(newName.length() < 1)
            return;

        auto widget = qobject_cast <ChatWidget *>(sender());
        if(!widget)
            return;

        bool accepted = server->setChangeName(prevName, newName);
        if (!accepted)
        {
            QTcpSocket *targetSocket = chatList.key(widget);
            server->sendNameChangeRejected(targetSocket);
            return;
        }

        int index = ui->twChat->indexOf(widget);

        if (index != -1)
            ui->twChat->setTabText(index, newName);
    });

    connect(chatWidget, &ChatWidget::initSendFile, this, &MainWindow::onInitSendFile);
    connect(chatWidget, &ChatWidget::sendMessage, server, &Server::sendMessage);
}

void MainWindow::clientConnectedToServer(QTcpSocket *socket)  // client
{
    this->socket = socket;

    dialog->clientConnected();

    chatWidget = new ChatWidget(this, socket, false);

    ui->twChat->addTab(chatWidget, QString("Server"));

    connect(chatWidget, &ChatWidget::isTyping, this, [this](){

        this->statusBar()->showMessage("Server is typing...", 700);
    });

    connect(chatWidget, &ChatWidget::initSendFile, this, &MainWindow::onInitSendFile);
}

void MainWindow::serverConnection()
{
    if(!isServerActive)
    {
        server = new Server(this);

        connect(server, &Server::newClientConnected, this, &MainWindow::newClientConnected);
        connect(this, &MainWindow::clientNameChanged, server, &Server::setChangeName);
        connect(server, &Server::sendClientDisconnected, this, [this](QTcpSocket *socket){
            ChatWidget *widget = chatList.value(socket);
            if(!widget)
                return;

            int index = ui->twChat->indexOf(widget);

            if (index != -1)
                ui->twChat->removeTab(index);

            chatList.remove(socket);
            widget->deleteLater();
        });

        ui->lblStatus->setText("Running as Server");

        isServerActive = true;
    }
    else
    {
        QMessageBox msgBox;

        msgBox.setWindowTitle("Error");
        msgBox.setText("Server is On");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::clientConnection()
{
    if(!isClientActive)
    {
        clientManager = new ClientManager(this);

        connect(clientManager, &ClientManager::clientConnectedToServer, this, &MainWindow::clientConnectedToServer);

        ui->lblStatus->setText("Connected as Client");

        isClientActive = true;
        ui->actionServer_Mode->setEnabled(false);
    }
    else
    {
        QMessageBox msgBox;

        msgBox.setWindowTitle("Error");
        msgBox.setText("Client is On");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::on_btnSetting_clicked()
{
    if(dialog == nullptr)
    {
        dialog = new SettingDialog(this);

        connect(dialog, &SettingDialog::connectionIsSelected, this, [this](bool isServer){
            if(isServer)
                serverConnection();
            else
                clientConnection();
        });

        connect(dialog, &SettingDialog::nameChange, this, [this](QString name){
            if (chatWidget)
                chatWidget->nameChange(name);
        });
    }
    dialog->show();
}


