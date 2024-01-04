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

void MainWindow::OnInitSendFile(QString path, qint64 size)
{
    QString text = QString("name file: %1\n size: %2").arg(path).arg(size);;

    auto result = QMessageBox::question(this, "Init send file", text);

    auto widget = dynamic_cast<ChatWidget*>(sender());
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
    QString name = QString("Chat %1").arg(socket->property("id").toString());

    socket->setProperty("name", name);

    ChatWidget *chatWidget = new ChatWidget(socket, this, true);
    _serverChatWidget = chatWidget;

    ui->twChat->addTab(chatWidget, name);

    connect(_serverChatWidget, &ChatWidget::isTyping, this, [this, socket](){

        QString name = socket->property("name").toString();
        if(name.length() < 1)
            name = QString("Client %1").arg(socket->property("id").toInt());

        this->statusBar()->showMessage(QString("%1 is typing...").arg(name), 700);
    });

    connect(chatWidget, &ChatWidget::nameChanged, this, [this, socket](QString name){
        if(name.length() < 1)
            return;

        ui->twChat->setTabText(socket->property("id").toInt() -1, name);
    });

    connect(_serverChatWidget, &ChatWidget::initSendFile, this, &MainWindow::OnInitSendFile);
}

void MainWindow::clientConnectedToServer(QTcpSocket *socket)  // client
{
    ChatWidget *chatWidget = new ChatWidget(socket);
    _clientChatWidget = chatWidget;

    ui->twChat->addTab(chatWidget, QString("Server"));

    connect(_clientChatWidget, &ChatWidget::isTyping, this, [this](){
        this->statusBar()->showMessage("Server is typing...", 700);
    });

    connect(_clientChatWidget, &ChatWidget::initSendFile, this, &MainWindow::OnInitSendFile);
}


