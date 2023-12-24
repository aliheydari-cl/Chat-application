#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    Server *server = new Server(this, true);
    connect(server, &Server::server_newClientConnected, this, &MainWindow::newClientconnected);

    ui->lblStatus->setText("Server Mod");

}

void MainWindow::on_actionClient_mode_triggered()
{
    if(ui->lblStatus->text() == "Server Mod")
    {
        qDebug() << "Error";
        return;
    }
    Server *server = new Server(this, false);
    connect(server, &Server::server_ConnectedToServer, this, &MainWindow::clientConnnectedToServer);

    ui->lblStatus->setText("Client Mod");

}


void MainWindow::newClientconnected(QTcpSocket *client)
{
    ChatWidget *chatWidget = new ChatWidget(client);
    ui->twChat->addTab(chatWidget, QString("Chat %1").arg(client->property("id").toString()));
}

void MainWindow::clientConnnectedToServer(QTcpSocket *client)
{
    ChatWidget *chatWidget = new ChatWidget(client);
    ui->twChat->addTab(chatWidget, QString("Chat %1").arg(client->property("id").toString()));
}




