/*
 * PasswordDialog.cpp
 *
 *  Created on: 24 March 2012
 *      Author: Max Foster
 */

#include "PasswordDialog.h"
#include "ui_PasswordDialog.h"

#include "Setting.h"
#include "SettingController.h"
#include "dialogs/setting/SettingForm.h"
#include "Utils.h"

PasswordDialog::PasswordDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::PasswordDialog), tries(0)
{
    ui->setupUi(this);
    password = SettingController::getSetting(SettingForm::keyProgramPassword).getValue();

    connect(this, SIGNAL(noPasswordSet()), this, SLOT(accept()), Qt::QueuedConnection);
    if (password.empty()) emit noPasswordSet();
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::on_pushButton_submit_clicked()
{
    ++tries;
    if (ui->lineEdit->text() == password.c_str()) done(Accepted);
    else
    {
        ui->lineEdit->setStyleSheet("QLineEdit { background-color: red; }");
        ui->lineEdit->setToolTip("");

        if (tries >= 3)
        {
            showInfoDialog("3 failed attempts. Access denied");
            done(Rejected);
        }
    }
}

void PasswordDialog::on_pushButton_cancel_clicked()
{
    done(Rejected);
}
