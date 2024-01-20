#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <server.h>
#include <chatwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void newClientConnected(QTcpSocket *client);
    void clientConnectedToServer(QTcpSocket *client);
    void on_actionServer_Mode_triggered();
    void on_actionClient_mode_triggered();
    void onInitSendFile(QString path, qint64 size);
    void onRejectionSendFile();

signals:
    void fileInitRejected();
    void clientNameChanged(QString, QString);


private:
    Ui::MainWindow *ui;
    ChatWidget *_clientChatWidget;
    ChatWidget *_serverChatWidget;

    bool isServerActive = false;
    bool isClientActive = false;

    Server *_server;

    QMap<QTcpSocket *, ChatWidget *> chatList;

};
#endif // MAINWINDOW_H
