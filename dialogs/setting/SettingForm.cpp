/*
 * SettingForm.cpp
 *
 *  Created on: 23 March 2012
 *      Author: Max Foster
 */

#include "SettingForm.h"
#include "ui_SettingForm.h"

#include <QDir>
#include <QFileDialog>

#include "Setting.h"
#include "SettingController.h"
#include "Utils.h"
#include "Databases.h"

const char
*SettingForm::keyDatabaseDirectory = "database directory",
*SettingForm::keyBackupDirectory = "backup directory",
*SettingForm::keyInvoiceSubject = "invoice subject",
*SettingForm::keyInvoiceBody = "invoice body",
*SettingForm::keyReceiptSubject = "receipt subject",
*SettingForm::keyReceiptBody = "receipt body",
*SettingForm::keyReminderSubject = "reminder subject",
*SettingForm::keyReminderBody = "reminder body",
*SettingForm::keyEmailUsername = "email username",
*SettingForm::keyEmailPassword = "email password",
*SettingForm::keyEmailHost = "email host",
*SettingForm::keyEmailPort = "email port";

SettingForm::SettingForm(QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingForm)
{
    ui->setupUi(this);
    populateInputs();
}

SettingForm::~SettingForm()
{
    delete ui;
}

void SettingForm::on_pushButton_save_clicked()
{
    if (inputsAreValid() && updateSettings()) done(Accepted);
}

void SettingForm::on_pushButton_cancel_clicked()
{
    done(Rejected);
}

void SettingForm::on_lineEdit_databaseDirectory_textEdited(const QString &value)
{
    if (QDir(value).exists() && (value.length() > 0))
    {
        ui->lineEdit_databaseDirectory->setStyleSheet("");
        ui->lineEdit_databaseDirectory->setToolTip("");
    }
    else
    {
        ui->lineEdit_databaseDirectory->setStyleSheet("QLineEdit { background-color: red; }");
        ui->lineEdit_databaseDirectory->setToolTip("Directory does not exist");
        ui->tabWidget->setCurrentIndex(0);
    }
}

void SettingForm::on_pushButton_browseDatabaseDirectory_clicked()
{
    QString directoryName = QFileDialog::getExistingDirectory(this, "Directory to save database files in");
    ui->lineEdit_databaseDirectory->setText(directoryName);
    on_lineEdit_databaseDirectory_textEdited(ui->lineEdit_databaseDirectory->text());
}

void SettingForm::on_lineEdit_backupDirectory_textEdited(const QString &value)
{
    if (QDir(value).exists() && (value.length() > 0))
    {
        ui->lineEdit_backupDirectory->setStyleSheet("");
        ui->lineEdit_backupDirectory->setToolTip("");
    }
    else
    {
        ui->lineEdit_backupDirectory->setStyleSheet("QLineEdit { background-color: red; }");
        ui->lineEdit_backupDirectory->setToolTip("Directory does not exist");
        ui->tabWidget->setCurrentIndex(0);
    }
}

void SettingForm::on_pushButton_browseBackupDirectory_clicked()
{
    QString directoryName = QFileDialog::getExistingDirectory(this, "Directory to save backups in");
    ui->lineEdit_backupDirectory->setText(directoryName);
    on_lineEdit_backupDirectory_textEdited(ui->lineEdit_backupDirectory->text());
}

void SettingForm::populateInputs()
{
    Database<Setting>::recordListPtr settings = SettingController::getAllSettings();
    for (unsigned i = 0; i < settings->size(); ++i)
    {
        Setting &setting = settings->at(i);
        const char *key = setting.getKey();

        if (strcmp(key, keyDatabaseDirectory) == 0) ui->lineEdit_databaseDirectory->setText(setting.getValue());
        else if (strcmp(key, keyBackupDirectory) == 0) ui->lineEdit_backupDirectory->setText(setting.getValue());
        else if (strcmp(key, keyInvoiceSubject) == 0) ui->lineEdit_invoiceSubject->setText(setting.getValue());
        else if (strcmp(key, keyInvoiceBody) == 0) ui->plainTextEdit_invoiceBody->setPlainText(setting.getValue());
        else if (strcmp(key, keyReceiptSubject) == 0) ui->lineEdit_receiptSubject->setText(setting.getValue());
        else if (strcmp(key, keyReceiptBody) == 0) ui->plainTextEdit_receiptBody->setPlainText(setting.getValue());
        else if (strcmp(key, keyReminderSubject) == 0) ui->lineEdit_reminderSubject->setText(setting.getValue());
        else if (strcmp(key, keyReminderBody) == 0) ui->plainTextEdit_reminderBody->setPlainText(setting.getValue());
        else if (strcmp(key, keyEmailUsername) == 0) ui->lineEdit_emailUsername->setText(setting.getValue());
        else if (strcmp(key, keyEmailPassword) == 0) ui->lineEdit_emailPassword->setText(setting.getValue());
        else if (strcmp(key, keyEmailHost) == 0) ui->lineEdit_emailHostAddress->setText(setting.getValue());
        else if (strcmp(key, keyEmailPort) == 0) ui->spinBox_emailHostPort->setValue(atoi(setting.getValue()));
    }
}

bool SettingForm::inputsAreValid()
{
    on_lineEdit_databaseDirectory_textEdited(ui->lineEdit_databaseDirectory->text());
    if (ui->lineEdit_databaseDirectory->styleSheet() != "") return false;

    on_lineEdit_backupDirectory_textEdited(ui->lineEdit_backupDirectory->text());
    if (ui->lineEdit_backupDirectory->styleSheet() != "") return false;

    return true;
}

bool SettingForm::updateSettings()
{
    std::string previousDatabaseDirectory = Databases::customers().databaseDirectory(),
            previousBackupDirectory = Databases::customers().backupDirectory();

    const unsigned settingCount = 12;
    Setting newSettings[] = {
        Setting(keyDatabaseDirectory, ui->lineEdit_databaseDirectory->text().toStdString().c_str()),
        Setting(keyBackupDirectory, ui->lineEdit_backupDirectory->text().toStdString().c_str()),
        Setting(keyInvoiceSubject, ui->lineEdit_invoiceSubject->text().toStdString().c_str()),
        Setting(keyInvoiceBody, ui->plainTextEdit_invoiceBody->toPlainText().toStdString().c_str()),
        Setting(keyReceiptSubject, ui->lineEdit_receiptSubject->text().toStdString().c_str()),
        Setting(keyReceiptBody, ui->plainTextEdit_receiptBody->toPlainText().toStdString().c_str()),
        Setting(keyReminderSubject, ui->lineEdit_reminderSubject->text().toStdString().c_str()),
        Setting(keyReminderBody, ui->plainTextEdit_reminderBody->toPlainText().toStdString().c_str()),
        Setting(keyEmailUsername, ui->lineEdit_emailUsername->text().toStdString().c_str()),
        Setting(keyEmailPassword, ui->lineEdit_emailPassword->text().toStdString().c_str()),
        Setting(keyEmailHost, ui->lineEdit_emailHostAddress->text().toStdString().c_str()),
        Setting(keyEmailPort, toString(ui->spinBox_emailHostPort->value()).c_str())
    };

    try { for (unsigned i = 0; i < settingCount; ++i) newSettings[i].validate(); }
    catch (const std::exception &e) { return false; }

    Database<Setting>::recordListPtr settings = SettingController::getAllSettings();

    // Update existing settings
    for (unsigned i = 0; i < settings->size(); ++i)
    {
        // Try to find two matching settings so that the one in the database can be updated
        Setting &setting = settings->at(i);
        Setting *matchingSetting = NULL;
        for (unsigned j = 0; j < settingCount; ++j)
        {
            if (strcmp(setting.getKey(), newSettings[j].getKey()) == 0) matchingSetting = &newSettings[j];
        }

        if (matchingSetting == NULL) continue;
        setting.setValue(matchingSetting->getValue());
        SettingController::Update(setting, this);

        // Copy ID over to indicate to the next loop that the setting already exists
        *static_cast<Record *>(matchingSetting) = *static_cast<Record *>(&setting);
    }

    // Create new settings for settings that do not exist already
    for (unsigned i = 0; i < settingCount; ++i)
    {
        if (newSettings[i].null()) SettingController::Create(newSettings[i], this);
    }

    try { Databases::reloadDatabaseFilenames(); }
    catch (const std::exception &e)
    {
        Setting databaseDirectorySetting = SettingController::getSetting(keyDatabaseDirectory),
                backupDirectorySetting = SettingController::getSetting(keyBackupDirectory);

        databaseDirectorySetting.setValue(previousDatabaseDirectory.c_str());
        backupDirectorySetting.setValue(previousBackupDirectory.c_str());

        SettingController::Update(databaseDirectorySetting, this);
        SettingController::Update(backupDirectorySetting, this);

        Databases::reloadDatabaseFilenames();
    }

    return true;
}
