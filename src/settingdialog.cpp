#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::clientConnected()
{
    ui->btnSave->setEnabled(true);
    ui->leName->setEnabled(true);
    ui->comboBox->setEnabled(false);
}

void SettingDialog::serverConnected()
{
    ui->btnSave->setEnabled(false);
    ui->leName->setEnabled(false);
    ui->comboBox->setEnabled(false);
}


void SettingDialog::on_btnSave_clicked()
{
    QString name = ui->leName->text().trimmed();
    ui->leName->setText("");

    if(name == "")
        return;

    this->close();
    emit nameChange(name);
}


void SettingDialog::on_comboBox_activated(int index)
{
    if(index == 0)
    {
        emit connectionIsSelected(true);
        this->close();
    }
    else
    {
        emit connectionIsSelected(false);
        this->close();
    }
}

