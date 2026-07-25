#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include "protocol.h"
#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();
    void clientConnected();
    void serverConnected();

signals:
    void connectionIsSelected(bool isServer);
    void nameChange(QString name);

private slots:
    void on_btnSave_clicked();

    void on_comboBox_activated(int index);

private:
    Ui::SettingDialog *ui;
    Protocol protocol;
    QTcpSocket *socket;
};

#endif // SETTINGDIALOG_H
