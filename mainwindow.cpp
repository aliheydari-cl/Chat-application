#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

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

void MainWindow::on_actionServer_Mode_triggered()
{
    if(!isServerActive)
    {
        Server *server = new Server(this, true);
        _server = server;

        connect(server, &Server::newClientConnected, this, &MainWindow::newClientConnected);

        ui->lblStatus->setText("Server Mod");

        isServerActive = true;
        ui->actionClient_mode->setEnabled(false);
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

void MainWindow::on_actionClient_mode_triggered()
{
    if(!isClientActive)
    {
        Server *server = new Server(this, false);
        _server = server;

        connect(server, &Server::connectedToServer, this, &MainWindow::clientConnectedToServer);

        ui->lblStatus->setText("Client Mod");

        isClientActive = true;
        ui->actionServer_Mode->setEnabled(false);
    }
    else
    {
        QMessageBox msgBox;

        msgBox.setWindowTitle("Error");
        msgBox.setText("Cleint is On");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::onInitSendFile(QString path, qint64 size)
{
    QString text = QString("name file: %1\n size: %2").arg(path).arg(size);;

    auto result = QMessageBox::question(this, "Init send file", text);

    auto widget = dynamic_cast<ChatWidget *>(sender());

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
    ChatWidget *chatWidget = new ChatWidget(this, socket, true);
    _serverChatWidget = chatWidget;

    chatList[socket] = chatWidget;

    ui->twChat->addTab(chatWidget, socket->property("name").toString());

    connect(_serverChatWidget, &ChatWidget::isTyping, this, [this, socket](){

        QString name = socket->property("name").toString();
        if(name.length() < 1)
            name = QString("Client %1").arg(socket->property("id").toInt());

        this->statusBar()->showMessage(QString("%1 is typing...").arg(name), 700);
    });

    connect(chatWidget, &ChatWidget::nameChanged, this, [this](QString prevName, QString newName){

        if(newName.length() < 1)
            return;

        auto widget = qobject_cast <ChatWidget *>(sender());
        int index = ui->twChat->indexOf(widget);

        ui->twChat->setTabText(index, newName);

        emit clientNameChanged(prevName, newName);
    });

    connect(_server, &Server::sendClientDisconnected, this, [this](QTcpSocket *socket){
        ChatWidget *widget = chatList.value(socket);

        int index = ui->twChat->indexOf(widget);

        ui->twChat->removeTab(index);
        chatList.remove(socket);
    });

    connect(_serverChatWidget, &ChatWidget::initSendFile, this, &MainWindow::onInitSendFile);
    connect(this, &MainWindow::clientNameChanged, _server, &Server::setChangeName);
    connect(_serverChatWidget, &ChatWidget::sendMessage, _server, &Server::sendMessage);
}

void MainWindow::clientConnectedToServer(QTcpSocket *socket)  // client
{
    ChatWidget *chatWidget = new ChatWidget(this, socket, false);
    _clientChatWidget = chatWidget;

    ui->twChat->addTab(chatWidget, QString("Server"));

    connect(_clientChatWidget, &ChatWidget::isTyping, this, [this](){

        this->statusBar()->showMessage("Server is typing...", 700);
    });

    connect(_clientChatWidget, &ChatWidget::initSendFile, this, &MainWindow::onInitSendFile);
}


