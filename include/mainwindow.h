#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingdialog.h"
#include <QMainWindow>
#include <server.h>
#include <chatwidget.h>
#include <clientmanager.h>

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

    void serverConnection();
    void clientConnection();

    void onInitSendFile(QString path, qint64 size);
    void onRejectionSendFile();

    void on_btnSetting_clicked();

signals:
    void fileInitRejected();
    void clientNameChanged(QString, QString);


private:
    Ui::MainWindow *ui;
    ChatWidget *chatWidget = nullptr;

    bool isServerActive = false;
    bool isClientActive = false;

    Server *server = nullptr;
    QTcpSocket *socket = nullptr;
    SettingDialog *dialog = nullptr;
    ClientManager* clientManager = nullptr;
    QString myName;

    QMap<QTcpSocket *, ChatWidget *> chatList;

};
#endif // MAINWINDOW_H
