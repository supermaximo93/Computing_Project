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
*SettingForm::keyPdfDirectory = "pdf directory",
*SettingForm::keyInvoiceSubject = "invoice subject",
*SettingForm::keyInvoiceBody = "invoice body",
*SettingForm::keyReceiptSubject = "receipt subject",
*SettingForm::keyReceiptBody = "receipt body",
*SettingForm::keyReminderSubject = "reminder subject",
*SettingForm::keyReminderBody = "reminder body",
*SettingForm::keyEmailUsername = "email username",
*SettingForm::keyEmailPassword = "email password",
*SettingForm::keyEmailHost = "email host",
*SettingForm::keyEmailPort = "email port",
*SettingForm::keyProgramPassword = "program password";

Setting SettingForm::getDatabaseDirectory()
{
    return SettingController::getSetting(keyDatabaseDirectory);
}

Setting SettingForm::getBackupDirectory()
{
    return SettingController::getSetting(keyBackupDirectory);
}

Setting SettingForm::getPdfDirectory()
{
    return SettingController::getSetting(keyPdfDirectory);
}

QString SettingForm::getPdfDirectoryWithoutSlash()
{
    QString pdfDirectory = SettingController::getSetting(keyPdfDirectory).getValue();

    if ((pdfDirectory[pdfDirectory.length() - 1] == '/') || (pdfDirectory[pdfDirectory.length() - 1] == '\\'))
        pdfDirectory.chop(1);

    return pdfDirectory + "/pdfs";
}

SettingForm::SettingForm(QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingForm)
{
    ui->setupUi(this);
    populateInputs();

    // Hide the cancel button if there are no settings (i.e. if it's a first time run). The user needs to save some
    // settings so don't allow them to cancel out of it
    ui->pushButton_cancel->setHidden(SettingController::getAllSettings()->empty());
}

SettingForm::~SettingForm()
{
    delete ui;
}

void SettingForm::closeEvent(QCloseEvent *event)
{
    // If cancel button is hidden then it means that there are no settings (i.e. it's a first time run) so don't let
    // the user quit
    if (ui->pushButton_cancel->isHidden()) event->ignore();
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

void SettingForm::on_lineEdit_pdfDirectory_textEdited(const QString &value)
{
    if (QDir(value).exists() && (value.length() > 0))
    {
        ui->lineEdit_pdfDirectory->setStyleSheet("");
        ui->lineEdit_pdfDirectory->setToolTip("");
    }
    else
    {
        ui->lineEdit_pdfDirectory->setStyleSheet("QLineEdit { background-color: red; }");
        ui->lineEdit_pdfDirectory->setToolTip("Directory does not exist");
        ui->tabWidget->setCurrentIndex(0);
    }
}

void SettingForm::on_pushButton_browsePdfDirectory_clicked()
{
    QString directoryName = QFileDialog::getExistingDirectory(this, "Directory to save PDF files in");
    ui->lineEdit_pdfDirectory->setText(directoryName);
    on_lineEdit_pdfDirectory_textEdited(ui->lineEdit_pdfDirectory->text());
}

void SettingForm::on_pushButton_saveNewPassword_clicked()
{
    const int minPasswordLength = 4,
            maxPasswordLength = Setting::maxValueLength,
            newPasswordLength = ui->lineEdit_newPassword->text().length();

    Setting passwordSetting = SettingController::getSetting(keyProgramPassword);
    if ((ui->lineEdit_oldPassword->text() == passwordSetting.getValue()) || passwordSetting.null())
    {
        ui->lineEdit_oldPassword->setStyleSheet("");
        ui->lineEdit_oldPassword->setToolTip("");

        if ((newPasswordLength < minPasswordLength) || (newPasswordLength > maxPasswordLength))
        {
            ui->lineEdit_newPassword->setStyleSheet("QLineEdit { background-color: red; }");
            ui->lineEdit_newPassword->setToolTip(("Password must be between " + toString(minPasswordLength) + " and "
                                                  + toString(maxPasswordLength) + " characters").c_str());
            return;
        }
        else
        {
            ui->lineEdit_newPassword->setStyleSheet("");
            ui->lineEdit_newPassword->setToolTip("");
        }

        if (ui->lineEdit_newPassword->text() == ui->lineEdit_confirmNewPassword->text())
        {
            ui->lineEdit_confirmNewPassword->setStyleSheet("");
            ui->lineEdit_confirmNewPassword->setToolTip("");

            passwordSetting.setValue(ui->lineEdit_newPassword->text().toStdString().c_str());
            if (passwordSetting.null())
            {
                passwordSetting.setKey(keyProgramPassword);
                SettingController::Create(passwordSetting, this);
            }
            else SettingController::Update(passwordSetting, this);

            ui->lineEdit_oldPassword->setPlaceholderText("");
        }
        else
        {
            ui->lineEdit_confirmNewPassword->setStyleSheet("QLineEdit { background-color: red; }");
            ui->lineEdit_confirmNewPassword->setToolTip("New password and confirmation do not match");
        }
    }
    else
    {
        ui->lineEdit_oldPassword->setStyleSheet("QLineEdit { background-color: red; }");
        ui->lineEdit_oldPassword->setToolTip("Password is incorrect");
    }
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
        else if (strcmp(key, keyPdfDirectory) == 0) ui->lineEdit_pdfDirectory->setText(setting.getValue());
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
        else if ((strcmp(key, keyProgramPassword) == 0) && (strlen(setting.getValue()) == 0))
            ui->lineEdit_oldPassword->setPlaceholderText("No password set");
    }

    if (SettingController::getSetting(keyProgramPassword).null())
        ui->lineEdit_oldPassword->setPlaceholderText("No password set");
}

bool SettingForm::inputsAreValid()
{
    on_lineEdit_databaseDirectory_textEdited(ui->lineEdit_databaseDirectory->text());
    if (ui->lineEdit_databaseDirectory->styleSheet() != "") return false;

    on_lineEdit_backupDirectory_textEdited(ui->lineEdit_backupDirectory->text());
    if (ui->lineEdit_backupDirectory->styleSheet() != "") return false;

    on_lineEdit_pdfDirectory_textEdited(ui->lineEdit_pdfDirectory->text());
    if (ui->lineEdit_pdfDirectory->styleSheet() != "") return false;

    return true;
}

bool SettingForm::updateSettings()
{
    std::string previousDatabaseDirectory = Databases::customers().databaseDirectory(),
            previousBackupDirectory = Databases::customers().backupDirectory(),
            previousPdfDirectory = Databases::customers().pdfDirectory();

    const unsigned settingCount = 10;
    Setting newSettings[] = {
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

    Setting databaseDirectorySetting(keyDatabaseDirectory,
                                     ui->lineEdit_databaseDirectory->text().toStdString().c_str()),
            backupDirectorySetting(keyBackupDirectory, ui->lineEdit_backupDirectory->text().toStdString().c_str()),
            pdfDirectorySetting(keyPdfDirectory, ui->lineEdit_pdfDirectory->text().toStdString().c_str());

    SettingController::Create(databaseDirectorySetting, this);
    SettingController::Create(backupDirectorySetting, this);
    SettingController::Create(pdfDirectorySetting, this);

    try { Databases::reloadDatabaseFilenames(); }
    catch (const std::exception &e)
    {
        databaseDirectorySetting.setValue(previousDatabaseDirectory.c_str());
        backupDirectorySetting.setValue(previousBackupDirectory.c_str());
        pdfDirectorySetting.setValue(previousPdfDirectory.c_str());

        SettingController::Update(databaseDirectorySetting, this);
        SettingController::Update(backupDirectorySetting, this);
        SettingController::Update(pdfDirectorySetting, this);

        Databases::reloadDatabaseFilenames();
    }

    return true;
}

