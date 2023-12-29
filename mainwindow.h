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
    void newClientconnected(QTcpSocket *client);

    void clientConnnectedToServer(QTcpSocket *client);

    void on_actionServer_Mode_triggered();

    void on_actionClient_mode_triggered();

private:
    Ui::MainWindow *ui;

    bool serverIsOn = false;
    bool clentIsOn = false;

};
#endif // MAINWINDOW_H
