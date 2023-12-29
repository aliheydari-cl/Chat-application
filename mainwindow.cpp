#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    if(!serverIsOn)
    {
        Server *server = new Server(this, true);
        connect(server, &Server::server_newClientConnected, this, &MainWindow::newClientconnected);
        ui->lblStatus->setText("Server Mod");

        serverIsOn = true;
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
    if(!clentIsOn)
    {

        Server *server = new Server(this, false);
        connect(server, &Server::server_ConnectedToServer, this, &MainWindow::clientConnnectedToServer);
        ui->lblStatus->setText("Client Mod");

        clentIsOn = true;
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


void MainWindow::newClientconnected(QTcpSocket *client)
{
    ChatWidget *chatWidget = new ChatWidget(client, this, true);
    ui->twChat->addTab(chatWidget, QString("Chat %1").arg(client->property("id").toString()));

    connect(chatWidget, &ChatWidget::isTyping, [this, client](){

        QString name = client->property("name").toString();
        if(name.length() < 1)
            name = QString("Client %1").arg(client->property("id").toInt());

        this->statusBar()->showMessage(QString("%1 is typing...").arg(name), 700);

    });

    connect(chatWidget, &ChatWidget::nameChanged, [this, client](QString name){
        QString nameTab = name;
        if(name.length() < 1)
            return;

        ui->twChat->setTabText(client->property("id").toInt() -1, name);
        client->setProperty("name", name);
    });


}

void MainWindow::clientConnnectedToServer(QTcpSocket *client)
{
    ChatWidget *chatWidget = new ChatWidget(client);
    ui->twChat->addTab(chatWidget, QString("Server"));
    connect(chatWidget, &ChatWidget::isTyping, [this](){
        this->statusBar()->showMessage("Server is typing...", 700);
    });

}




